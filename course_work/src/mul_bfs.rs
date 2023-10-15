use std::collections::HashMap;
use std::hash::{Hash};
use std::ops::Index;

use std::sync::{Arc, RwLock};
use rand::prelude::*;
use crate::types::{PathSize};

pub type ArcNode<T> = Arc<T>;
pub type SinglePath<T> = Arc<RwLock<im::Vector<ArcNode<T>>>>;
pub type ResultPath<T> = Arc<RwLock<Option<SinglePath<T>>>>;
pub type Paths<T> = Arc<RwLock<Vec<SinglePath<T>>>>;
pub type NodesMap<T> = Arc<RwLock<HashMap<ArcNode<T>, Vec<ArcNode<T>>>>>;
pub type ArcGraph<T> = Arc<HashMap<ArcNode<T>, Vec<ArcNode<T>>>>;
pub type CommunicationMarker = Arc<RwLock<bool>>;

pub fn find_path<T>(start_node: ArcNode<T>, end_node: ArcNode<T>, graph: ArcGraph<T>, paths_size: PathSize, total_threads: usize)
                    -> Option<im::Vector<ArcNode<T>>>
    where T: Eq + std::hash::Hash + Send + Sync {

    let result_path = Arc::new(RwLock::new(None));

    std::thread::scope(|s| {
        let paths = Arc::new(RwLock::new(vec![
            Arc::new(RwLock::new(im::Vector::from(vec![start_node.clone()])))
        ]));
        let comm_mark = Arc::new(RwLock::new(true));
        for index in 0..total_threads {
            let mut task = ThreadTask::<T>::new(
                paths.clone(), graph.clone(), start_node.clone(), end_node.clone(), paths_size,
                result_path.clone(), comm_mark.clone());
            if index == total_threads - 1 {
                task.execute()
            } else {
                s.spawn(move || task.execute());
            }
        }
    });

    let result = std::mem::take(&mut *result_path.write().unwrap());
    if let Some(path) = result {
        return Some(std::mem::take(&mut *path.write().unwrap()));
    }
    None
}

struct ThreadTask<T> {
    paths: Paths<T>,
    graph: ArcGraph<T>,
    start_node: ArcNode<T>,
    end_node: ArcNode<T>,
    paths_size: PathSize,
    result_path: ResultPath<T>,
    comm_mark: CommunicationMarker
}

impl<T: Hash> ThreadTask<T> {
    fn new(paths: Paths<T>, graph: ArcGraph<T>, start_node: ArcNode<T>,
           end_node: ArcNode<T>, paths_size: PathSize,
           result_path: ResultPath<T>, comm_mark: CommunicationMarker) -> Self {
        Self { paths, graph, start_node, end_node, paths_size, result_path, comm_mark }
    }

    fn execute(&mut self) {
        if let PathSize::Limited(limit) = self.paths_size {
            if limit == 0 { return None; }
        }

        let mut graph_visited = HashMap::new();
        let mut rand_gen = ThreadRng::default();

        'main_loop: while self.comm_mark.read() {
            let paths_read_result = self.paths.try_read();
            if paths_read_result.is_err() { continue 'main_loop; }
            let paths_read = paths_read_result.unwrap();

            if paths_read.is_empty() {/* break end*/}

            let chosen_path_index = rand_gen.gen_range(0..paths_read.len());
            let chosen_path_read_result = paths_read.index(chosen_path_index).try_read();
            if chosen_path_read_result.is_err() { continue 'main_loop; }
            let chosen_path_read = chosen_path_read_result.unwrap();

            let path_last_node = chosen_path_read.last().unwrap();
            if Arc::ptr_eq(path_last_node, &self.end_node) {/* break end*/}

            let last_node_neighbours = &self.graph[path_last_node];
            if last_node_neighbours.is_empty() {
                paths.remove(chosen_path_index);
                continue 'main_loop;
            }

            let last_index = last_node_neighbours.len() - 1;
            let visited_nodes = {
                let is_contained_key = graph_visited.contains_key(path_last_node);
                if !is_contained_key { graph_visited.insert(path_last_node.clone(), vec![]); }
                graph_visited.get_mut(path_last_node).unwrap()
            };

            let mut is_any_unvisited = false;
            'sub_loop: for node in &last_node_neighbours[0..last_index] {
                if let PathSize::Limited(limit) = self.paths_size {
                    if paths.len() >= limit { break 'sub_loop; }
                }
                if visited_nodes.contains(node) { continue 'sub_loop; }
                visited_nodes.push(node.clone());
                let mut new_path = paths[chosen_path_index].clone();
                new_path.push_back(node.clone());
                paths.push(new_path);
                is_any_unvisited = true;
            }
            if !visited_nodes.contains(&last_node_neighbours[last_index]) {
                visited_nodes.push(last_node_neighbours[last_index].clone());
                paths[chosen_path_index].push_back(last_node_neighbours[last_index].clone());
                is_any_unvisited = true;
            }
            if !is_any_unvisited {
                paths.remove(chosen_path_index);
                continue 'main_loop;
            }
        };
    }

    fn remove_path(&mut self, path: SinglePath<T>) {
        let mut path_write = self.paths.write().unwrap();
        let position_opt = path_write.iter().position(
            |node| { Arc::ptr_eq(node, &path) });
        if let Some(position) = position_opt {
            path_write.remove(position);
        }
    }
}
