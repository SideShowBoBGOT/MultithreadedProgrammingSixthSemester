use std::collections::HashMap;
use std::fmt::Display;
use std::hash::{Hash, Hasher};

use std::sync::{Arc, RwLock};
use rand::prelude::*;
pub use crate::my_types::{Graph, SingleNode};
use crate::my_types::{CommunicationMarker, Paths, ResultPath, SinglePath};
use crate::my_types::NodesMap;

pub fn find_path<T>(from: SingleNode<T>, to: SingleNode<T>, graph: Graph<T>, total_threads: usize) -> Option<im::Vector<SingleNode<T>>>
    where T: Eq + std::hash::Hash + Send + Sync {

    let result_path = Arc::new(RwLock::new(None));

    std::thread::scope(|s| {
        let visited_nodes = Arc::new(RwLock::new(
            HashMap::from([(from.clone(), vec![])])));
        let paths = Arc::new(RwLock::new(vec![
            Arc::new(RwLock::new(im::Vector::from(vec![from.clone()])))
        ]));
        let comm_mark = Arc::new(RwLock::new(true));
        for index in 0..total_threads {
            let mut task = ThreadTask::<T>::new(
                visited_nodes.clone(), paths.clone(),
                graph.clone(), to.clone(), result_path.clone(), comm_mark.clone());
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

enum ChoosePathResult<T> {
    Locked,
    DeadEnd(SinglePath<T>),
    PathChosen((SinglePath<T>, Vec<SingleNode<T>>)),
    NoPaths,
    FinalPathFound(SinglePath<T>),
}

enum UpdatePathsResult<T> {
    DeadEnd(SinglePath<T>),
    Updated
}

struct ThreadTask<T> {
    visited_map: NodesMap<T>,
    paths: Paths<T>,
    graph: Graph<T>,
    end_node: SingleNode<T>,
    result_path: ResultPath<T>,
    comm_mark: CommunicationMarker
}

impl<T: Hash> ThreadTask<T> {
    fn new(visited_nodes: NodesMap<T>, paths: Paths<T>, graph: Graph<T>,
           end_node: SingleNode<T>, result_path: ResultPath<T>,
           comm_mark: CommunicationMarker) -> Self {
        Self { visited_map: visited_nodes, paths, graph, end_node, result_path, comm_mark }
    }

    fn execute(&mut self) {
        let mut rand_gen = ThreadRng::default();
        while *self.comm_mark.read().unwrap() {
            match self.choose_path(&mut rand_gen) {
                ChoosePathResult::Locked => {}
                ChoosePathResult::DeadEnd(path) => self.remove_path(path),
                ChoosePathResult::PathChosen((path, unvisited_nodes)) => {
                    match self.update_paths(path, unvisited_nodes) {
                        UpdatePathsResult::DeadEnd(chosen_path) => {
                            self.remove_path(chosen_path);
                        }
                        UpdatePathsResult::Updated => {}
                    }
                },
                ChoosePathResult::NoPaths => {
                    let mut mark_write = self.comm_mark.write().unwrap();
                    *mark_write = false;
                    break;
                },
                ChoosePathResult::FinalPathFound(path) => {
                    let mut mark_write = self.comm_mark.write().unwrap();
                    if *mark_write {
                        *mark_write = false;
                        let mut res_write = self.result_path.write().unwrap();
                        *res_write = Some(path);
                    }
                    break;
                }
            }
        }
    }

    fn update_paths(&mut self, chosen_path: SinglePath<T>, unvisited_nodes: Vec<SingleNode<T>>)
                    -> UpdatePathsResult<T> {

        let mut chosen_path_write = chosen_path.write().unwrap();
        let last_node = chosen_path_write.last().unwrap();

        let mut visited_map_write = self.visited_map.write().unwrap();
        let visited_nodes_opt = &visited_map_write.get(last_node);

        let sure_unvisited = if visited_nodes_opt.is_none() {
            unvisited_nodes
        } else {
            let visited_nodes = visited_nodes_opt.unwrap();
            unvisited_nodes.into_iter().filter(|neighbour_node| {
                return !visited_nodes.contains(neighbour_node);
            }).map(|n| n.clone()).collect::<Vec<_>>()
        };

        if sure_unvisited.is_empty() {
            return UpdatePathsResult::DeadEnd(chosen_path.clone());
        }

        if visited_nodes_opt.is_none() {
            visited_map_write.insert(last_node.clone(), vec![]);
        }

        let visited_nodes = visited_map_write.get_mut(last_node).unwrap();

        let mut paths_write = self.paths.write().unwrap();

        let last_index = sure_unvisited.len() - 1;
        for (i, node) in sure_unvisited.into_iter().enumerate() {
            visited_nodes.push(node.clone());

            if i == last_index {
                chosen_path_write.push_back(node);
            } else {
                let mut new_path = chosen_path_write.clone();
                new_path.push_back(node);
                paths_write.push(Arc::new(RwLock::new(new_path)));
            }
        }

        UpdatePathsResult::Updated
    }

    fn choose_path(&mut self, rand_gen: &mut ThreadRng) -> ChoosePathResult<T> {
        let paths_read = {
            let paths_try_read = self.paths.try_read();
            if paths_try_read.is_err() { return ChoosePathResult::Locked; }
            paths_try_read.unwrap()
        };

        if paths_read.is_empty() { return ChoosePathResult::NoPaths; }

        let visited_map_read = {
            let visited_map_try_read = self.visited_map.try_read();
            if visited_map_try_read.is_err() { return ChoosePathResult::Locked; }
            visited_map_try_read.unwrap()
        };

        let chosen_path = paths_read.iter().choose(rand_gen).unwrap();
        let chosen_path_read = {
            let chosen_path_try_read = chosen_path.try_read();
            if chosen_path_try_read.is_err() { return ChoosePathResult::Locked; }
            chosen_path_try_read.unwrap()
        };

        let path_last_node = chosen_path_read.last().unwrap();
        if *path_last_node == self.end_node {
            return ChoosePathResult::FinalPathFound(chosen_path.clone());
        }

        let last_node_neighbours = &self.graph[path_last_node];
        let visited_nodes_opt = visited_map_read.get(path_last_node);

        let unvisited_nodes = if visited_nodes_opt.is_none() {
            last_node_neighbours.clone()
        } else {
            let visited_nodes = visited_nodes_opt.unwrap();
            last_node_neighbours.iter().filter(|neighbour_node| {
                return !visited_nodes.contains(neighbour_node);
            }).map(|n| n.clone()).collect::<Vec<_>>()
        };

        if unvisited_nodes.is_empty() {
            ChoosePathResult::DeadEnd(chosen_path.clone())
        } else {
            ChoosePathResult::PathChosen((chosen_path.clone(), unvisited_nodes))
        }
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
