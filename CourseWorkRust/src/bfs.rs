use std::collections::HashMap;
use std::sync::{Arc, LockResult, RwLock};

use rand::prelude::*;

pub type SingleNode<T> = Arc<T>;
pub type Nodes<T> = Arc<RwLock<Vec<SingleNode<T>>>>;
pub type SinglePath<T> = Nodes<T>;
pub type Paths<T> = Arc<RwLock<Vec<SinglePath<T>>>>;
pub type Graph<T> = Arc<HashMap<SingleNode<T>, Vec<SingleNode<T>>>>;

struct BFS<T> {
    visited_nodes: Nodes<T>,
    paths: Paths<T>,
}

impl<T> BFS<T> {
    fn find_path(&mut self, from: Arc<T>, to: Arc<T>, graph: Arc<Graph<T>>, total_threads: usize) {
        let mut handles = Vec::new();
        for _ in 0..total_threads {
            let visited_nodes = self.visited_nodes.clone();
            let paths = self.paths.clone();
            handles.push(|| {

            })
        }

    }

    fn thread_task(visited_nodes: Arc<RwLock<Vec<Arc<T>>>>, paths: Arc<RwLock<Vec<RwLock<Vec<Arc<T>>>>>>, ) {

    }

}

struct ThreadTask<T> {
    visited_nodes: Nodes<T>,
    paths: Paths<T>,
    graph: Graph<T>,
    rand_gen: ThreadRng
}

impl<T> ThreadTask<T> {
    fn execute(&mut self) {
        loop {

        }
    }

    fn find_path_and_unvisited_node(&mut self)
        -> Result<(SinglePath<T>, SingleNode<T>), FindUnvisitedErr<T>> {

        let paths_read = self.paths.try_read();
        if paths_read.is_err() { return Err(FindUnvisitedErr::PathsLocked); }
        let p = paths_read.unwrap().choose(&mut self.rand_gen).unwrap();
        let p_read = p.read().unwrap();

        let unvisited_node = {
            let path_last_node = p_read.last().unwrap();
            let last_node_neighbours = &self.graph[path_last_node];
            last_node_neighbours.iter().find(|neighbour_node| {
                self.visited_nodes.read().unwrap().iter().find(|node| {
                    Arc::ptr_eq(node, neighbour_node) == false
                }).is_some()
            })
        };

        match unvisited_node {
            None => Err(FindUnvisitedErr::AllNeighboursVisited(p.clone())),
            Some(node) => Ok((p.clone(), node.clone()))
        }
    }
}

enum FindUnvisitedErr<T> {
    AllNeighboursVisited(SinglePath<T>),
    PathsLocked,
}