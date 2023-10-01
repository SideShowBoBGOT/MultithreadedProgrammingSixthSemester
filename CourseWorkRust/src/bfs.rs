use std::collections::{HashMap, HashSet};
use std::sync::{Arc, Mutex, RwLock};

use rand::prelude::*;

pub type SingleNode<T> = Arc<T>;
pub type Nodes<T> = Arc<RwLock<Vec<SingleNode<T>>>>;
pub type SinglePath<T> = Arc<RwLock<Vec<SingleNode<T>>>>;
pub type Paths<T> = Arc<RwLock<Vec<SinglePath<T>>>>;
pub type Graph<T> = Arc<HashMap<SingleNode<T>, Vec<SingleNode<T>>>>;

pub fn find_path<T>(from: SingleNode<T>, to: SingleNode<T>, graph: Arc<Graph<T>>, total_threads: usize)
    where T: Eq + std::hash::Hash + Send + Sync {

    std::thread::scope(|s| {
        let mut visited_vec = Vec::new();
        visited_vec.push(from.clone());
        let visited_nodes = Arc::new(RwLock::new(visited_vec));
        let paths = Arc::new(RwLock::new(
            vec![Arc::new(RwLock::new(vec![from]))]));

        for _ in 0..(total_threads - 1) {
            let mut task = ThreadTask::<T>::new(&visited_nodes, &paths, &graph, &to);
            s.spawn(move || {
                task.execute();
            });
        }
        let mut task = ThreadTask::<T>::new(&visited_nodes, &paths, &graph, &to);
        task.execute();
    });

}

enum FindUnvisitedResult<T> {
    PathAndNode((SinglePath<T>, Vec<SingleNode<T>>)),
    AllNeighboursVisited(SinglePath<T>),
    PathsLocked,
}

struct ThreadTask<T> {
    visited_nodes: Nodes<T>,
    paths: Paths<T>,
    graph: Arc<Graph<T>>,
    end_node: SingleNode<T>,
}

impl<T: Eq + std::hash::Hash> ThreadTask<T> {
    fn new(visited_nodes: &Nodes<T>, paths: &Paths<T>, graph: &Arc<Graph<T>>, end_node: &SingleNode<T>) -> Self {
        Self {
            visited_nodes: visited_nodes.clone(),
            paths: paths.clone(),
            graph: graph.clone(),
            end_node: end_node.clone(),
        }
    }

    fn execute(&mut self) {
        let mut rand_gen = ThreadRng::default();
        loop {
            match self.find_path_and_unvisited_node(&mut rand_gen) {
                FindUnvisitedResult::PathAndNode((path, nodes)) => self.update_paths(path, nodes),
                FindUnvisitedResult::AllNeighboursVisited(path) => self.remove_path(path),
                FindUnvisitedResult::PathsLocked => {}
            }
        }
    }

    fn update_paths(&mut self, path: SinglePath<T>, nodes: Vec<SingleNode<T>>) {
        if let Ok(mut visited_write) = self.visited_nodes.try_write() {
            let sure_unvisited = nodes.into_iter()
                .filter(|n| self.is_node_visited(n)).collect::<Vec<_>>();

            let mut paths_write = self.paths.write().unwrap();

            for (i, node) in sure_unvisited.into_iter().enumerate() {
                visited_write.push(node.clone());
                if i == 0 {
                    let mut p_write = path.write().unwrap();
                    p_write.push(node);
                } else {
                    let p_read = path.read().unwrap();
                    let mut new_path = p_read.clone();
                    new_path.push(node);
                    paths_write.push(Arc::new(RwLock::new(new_path)));
                }
            }
        }
    }

    fn find_path_and_unvisited_node(&mut self, rand_gen: &mut ThreadRng) -> FindUnvisitedResult<T> {

        let paths_read = self.paths.try_read();
        if paths_read.is_err() { return FindUnvisitedResult::PathsLocked; }

        let paths_read = paths_read.unwrap();
        let p = paths_read.choose(rand_gen).unwrap();
        let p_read = p.read().unwrap();

        let unvisited_nodes = {
            let path_last_node = p_read.last().unwrap();
            let last_node_neighbours = &self.graph[path_last_node];
            last_node_neighbours.iter().filter(|neighbour_node| {
                self.is_node_visited(neighbour_node)
            }).map(|n| n.clone()).collect::<Vec<_>>()
        };

        if unvisited_nodes.is_empty() {
            FindUnvisitedResult::AllNeighboursVisited(p.clone())
        } else {
            FindUnvisitedResult::PathAndNode((p.clone(), unvisited_nodes))
        }
    }

    fn remove_path(&mut self, path: SinglePath<T>) {
        if let Ok(mut path_write) = self.paths.write() {
            let position_opt = path_write.iter().position(
                |node| { Arc::ptr_eq(node, &path) });
            if let Some(position) = position_opt {
                path_write.remove(position);
            }
        }
    }

    fn is_node_visited(&self, node: &SingleNode<T>) -> bool {
        self.visited_nodes.read().unwrap().iter().find(|visited| {
            Arc::ptr_eq(visited, node)
        }).is_some()
    }
}