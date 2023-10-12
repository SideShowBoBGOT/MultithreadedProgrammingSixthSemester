use std::collections::HashMap;
use std::hash::{Hash};

use std::sync::{Arc, RwLock};
use rand::prelude::*;
use crate::types::{ArcGraph, ArcNode};
use crate::types::{CommunicationMarker, Paths, ResultPath, SinglePath};
use crate::types::NodesMap;

pub fn find_path<T>(from: ArcNode<T>, to: ArcNode<T>, graph: ArcGraph<T>, total_threads: usize) -> Option<im::Vector<ArcNode<T>>>
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

#[derive(Debug)]
enum BFSError {
    Locked
}

enum ChoosePathMarker<T> {
    DeadEnd(SinglePath<T>),
    PathChosen((SinglePath<T>, Vec<ArcNode<T>>)),
    NoPaths,
    FinalPathFound(SinglePath<T>),
}

enum UpdatePathsMarker<T> {
    DeadEnd(SinglePath<T>),
    Updated
}

struct ThreadTask<T> {
    visited_map: NodesMap<T>,
    paths: Paths<T>,
    graph: ArcGraph<T>,
    end_node: ArcNode<T>,
    result_path: ResultPath<T>,
    comm_mark: CommunicationMarker
}

impl<T: Hash> ThreadTask<T> {
    fn new(visited_nodes: NodesMap<T>, paths: Paths<T>, graph: ArcGraph<T>,
           end_node: ArcNode<T>, result_path: ResultPath<T>,
           comm_mark: CommunicationMarker) -> Self {
        Self { visited_map: visited_nodes, paths, graph, end_node, result_path, comm_mark }
    }

    fn execute(&mut self) {
        let mut rand_gen = ThreadRng::default();
        while *self.comm_mark.read().unwrap() {
            let marker = self.choose_path(&mut rand_gen);
            if marker.is_err() {continue;}
            match marker.unwrap() {
                ChoosePathMarker::DeadEnd(path) => self.remove_path(path),
                ChoosePathMarker::PathChosen((path, unvisited_nodes)) => {
                    match self.update_paths(path, unvisited_nodes) {
                        UpdatePathsMarker::DeadEnd(chosen_path) => {
                            self.remove_path(chosen_path);
                        }
                        UpdatePathsMarker::Updated => {}
                    }
                },
                ChoosePathMarker::NoPaths => {
                    let mut mark_write = self.comm_mark.write().unwrap();
                    *mark_write = false;
                    break;
                },
                ChoosePathMarker::FinalPathFound(path) => {
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

    fn update_paths(&mut self, chosen_path: SinglePath<T>, unvisited_nodes: Vec<ArcNode<T>>)
                    -> UpdatePathsMarker<T> {
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
            return UpdatePathsMarker::DeadEnd(chosen_path.clone());
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
        UpdatePathsMarker::Updated
    }

    fn choose_path(&mut self, rand_gen: &mut ThreadRng) -> Result<ChoosePathMarker<T>, BFSError> {
        let paths_read = self.paths.try_read().map_err(|_| BFSError::Locked)?;
        if paths_read.is_empty() { return Ok(ChoosePathMarker::NoPaths); }
        let visited_map_read = self.visited_map.try_read().map_err(|_| BFSError::Locked)?;
        let chosen_path = paths_read.iter().choose(rand_gen).unwrap();
        let chosen_path_read = chosen_path.try_read().map_err(|_| BFSError::Locked)?;
        let path_last_node = chosen_path_read.last().unwrap();
        if *path_last_node == self.end_node {
            return Ok(ChoosePathMarker::FinalPathFound(chosen_path.clone()));
        }
        let last_node_neighbours = &self.graph[path_last_node];
        let unvisited_nodes = if let Some(visited_nodes) = visited_map_read.get(path_last_node) {
            last_node_neighbours.iter().filter(|neighbour_node| {
                return !visited_nodes.contains(neighbour_node);
            }).map(|n| n.clone()).collect::<Vec<_>>()
        } else {
            last_node_neighbours.clone()
        };
        if unvisited_nodes.is_empty() {
            Ok(ChoosePathMarker::DeadEnd(chosen_path.clone()))
        } else {
            Ok(ChoosePathMarker::PathChosen((chosen_path.clone(), unvisited_nodes)))
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
