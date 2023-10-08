use std::collections::{HashMap, HashSet};
use std::ops::DerefMut;
use std::sync::{Arc, Mutex, RwLock};

use rand::prelude::*;

pub type SingleNode<T> = Arc<Mutex<T>>;
pub type Nodes<T> = Arc<RwLock<Vec<SingleNode<T>>>>;
pub type SinglePath<T> = Vec<SingleNode<T>>;
pub type ResultPath<T> = Arc<RwLock<Option<SinglePath<T>>>>;
pub type Paths<T> = Arc<RwLock<Vec<SinglePath<T>>>>;
pub type Graph<T> = Arc<HashMap<SingleNode<T>, Vec<SingleNode<T>>>>;
type CommunicationMarker = Arc<RwLock<bool>>;

pub fn find_path<T>(from: SingleNode<T>, to: SingleNode<T>, graph: Graph<T>, total_threads: usize)
    where T: Eq + std::hash::Hash + Send {

    std::thread::scope(|s| {
        let initial_path = vec![from.clone()];
        let result_path = Arc::new(RwLock::new(None));
        let visited_nodes = Arc::new(RwLock::new(initial_path.clone()));
        let paths = Arc::new(RwLock::new(vec![initial_path]));
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

}

enum ChoosePathResult<T> {
    Locked,
    DeadEnd(SinglePath<T>),
    PathChosen((SinglePath<T>, Vec<SingleNode<T>>)),
    NoPaths,
    FinalPathFound(SinglePath<T>),
}

struct ThreadTask<T> {
    visited_nodes: Nodes<T>,
    paths: Paths<T>,
    graph: Graph<T>,
    end_node: SingleNode<T>,
    result_path: ResultPath<T>,
    comm_mark: CommunicationMarker
}

impl<T: Eq + std::hash::Hash> ThreadTask<T> {
    fn new(visited_nodes: Nodes<T>, paths: Paths<T>, graph: Graph<T>,
           end_node: SingleNode<T>, result_path: ResultPath<T>,
           comm_mark: CommunicationMarker) -> Self {
        Self { visited_nodes, paths, graph, end_node, result_path, comm_mark }
    }

    fn execute(&mut self) {
        let mut rand_gen = ThreadRng::default();
        let is_do_work = true;
        while is_do_work {
            match self.find_path_and_unvisited_node(&mut rand_gen) {
                ChoosePathResult::Locked => {}
                ChoosePathResult::DeadEnd(path) => self.remove_path(path),
                ChoosePathResult::PathChosen((path, unvisited_nodes)) => {
                    self.update_paths(path, unvisited_nodes)
                },
                ChoosePathResult::NoPaths => {
                    let mut mark_write = self.comm_mark.write().unwrap();
                    *mark_write = false;
                    break;
                },
                ChoosePathResult::FinalPathFound(path) => {
                    let mut mark_write = self.comm_mark.write().unwrap();
                    if mark_write {
                        *mark_write = false;
                        let res_write = self.result_path.write().unwrap();
                        *res_write = Some(path);
                    }
                    break;
                }
            }
        }
    }

    // fn update_paths(&mut self, path: SinglePath<T>, nodes: Vec<SingleNode<T>>) {
    //     let mut visited_write = self.visited_nodes.write().unwrap();
    //     let sure_unvisited = nodes.into_iter()
    //         .filter(|n| !self.is_node_visited(n)).collect::<Vec<_>>();
    //
    //     if sure_unvisited.is_empty() {
    //         return self.remove_path(path);
    //     }
    //
    //     let mut paths_write = self.paths.write().unwrap();
    //
    //     for (i, node) in sure_unvisited.into_iter().enumerate() {
    //         visited_write.push(node.clone());
    //         if i == 0 {
    //             let mut p_write = path.write().unwrap();
    //             p_write.push(node);
    //         } else {
    //             let mut new_path = {
    //                 let p_read = path.read().unwrap();
    //                 p_read.clone()
    //             };
    //             new_path.push(node);
    //             paths_write.push(Arc::new(RwLock::new(new_path)));
    //         }
    //     }
    // }
    //
    // fn find_path_and_unvisited_node(&mut self, rand_gen: &mut ThreadRng) -> FindUnvisitedResult<T> {
    //
    //     let paths_read = self.paths.try_read();
    //     if paths_read.is_err() { return FindUnvisitedResult::PathsLocked; }
    //
    //     let paths_read = paths_read.unwrap();
    //     let p = paths_read.choose(rand_gen).unwrap();
    //     let p_read = p.read().unwrap();
    //
    //     let unvisited_nodes = {
    //         let path_last_node = p_read.last().unwrap();
    //         let last_node_neighbours = &self.graph[path_last_node];
    //         last_node_neighbours.iter().filter(|neighbour_node| {
    //             !self.is_node_visited(neighbour_node)
    //         }).map(|n| n.clone()).collect::<Vec<_>>()
    //     };
    //
    //     if unvisited_nodes.is_empty() {
    //         FindUnvisitedResult::AllNeighboursVisited(p.clone())
    //     } else {
    //         FindUnvisitedResult::PathChosen((p.clone(), unvisited_nodes))
    //     }
    // }
    //
    // fn remove_path(&mut self, path: SinglePath<T>) {
    //     let mut path_write = self.paths.write().unwrap();
    //     let position_opt = path_write.iter().position(
    //         |node| { Arc::ptr_eq(node, &path) });
    //     if let Some(position) = position_opt {
    //         path_write.remove(position);
    //     }
    // }
    //
    // fn is_node_visited(&self, node: &SingleNode<T>) -> bool {
    //     self.visited_nodes.read().unwrap().iter().find(|visited| {
    //         Arc::ptr_eq(visited, node)
    //     }).is_some()
    // }
}