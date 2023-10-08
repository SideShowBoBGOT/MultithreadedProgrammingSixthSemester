use std::collections::{HashMap};
use std::hash::{Hash, Hasher};
use std::sync::{Arc, RwLock};

use rand::prelude::*;

pub struct SingleNode<T>(Arc<RwLock<T>>);

impl<T> PartialEq<Self> for SingleNode<T> {
    fn eq(&self, other: &Self) -> bool {
        Arc::ptr_eq(&self.0, &other.0)
    }
}

impl<T> Eq for SingleNode<T> {}

impl<T> Clone for SingleNode<T> {
    fn clone(&self) -> Self {
        Self(self.0.clone())
    }
}

impl<T: Hash> Hash for SingleNode<T> {
    fn hash<H: Hasher>(&self, state: &mut H) {
        let read_inner = self.0.read().unwrap();
        read_inner.hash(state)
    }
}

pub type Nodes<T> = Arc<RwLock<im::Vector<SingleNode<T>>>>;
pub type SinglePath<T> = Arc<RwLock<im::Vector<SingleNode<T>>>>;
pub type ResultPath<T> = Arc<RwLock<Option<SinglePath<T>>>>;
pub type Paths<T> = Arc<RwLock<im::Vector<SinglePath<T>>>>;
pub type Graph<T> = Arc<HashMap<SingleNode<T>, Vec<SingleNode<T>>>>;
type CommunicationMarker = Arc<RwLock<bool>>;

pub fn find_path<T>(from: SingleNode<T>, to: SingleNode<T>, graph: Graph<T>, total_threads: usize)
    where T: Eq + std::hash::Hash + Send + Sync {

    std::thread::scope(|s| {

        let initial_path = im::Vector::from(vec![from.clone()]);
        let result_path = Arc::new(RwLock::new(None));
        let visited_nodes = Arc::new(RwLock::new(initial_path.clone()));
        let paths = Arc::new(RwLock::new(im::Vector::from(vec![visited_nodes.clone()])));
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
    visited_nodes: Nodes<T>,
    paths: Paths<T>,
    graph: Graph<T>,
    end_node: SingleNode<T>,
    result_path: ResultPath<T>,
    comm_mark: CommunicationMarker
}

impl<T: Hash> ThreadTask<T> {
    fn new(visited_nodes: Nodes<T>, paths: Paths<T>, graph: Graph<T>,
           end_node: SingleNode<T>, result_path: ResultPath<T>,
           comm_mark: CommunicationMarker) -> Self {
        Self { visited_nodes, paths, graph, end_node, result_path, comm_mark }
    }

    fn execute(&mut self) {
        let mut rand_gen = ThreadRng::default();
        let is_do_work = true;
        while is_do_work {
            match self.choose_path(&mut rand_gen) {
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

        let mut visited_write = self.visited_nodes.write().unwrap();
        let sure_unvisited = unvisited_nodes.into_iter()
            .filter(|n| !visited_write.contains(n)).collect::<Vec<_>>();

        if sure_unvisited.is_empty() {
            return UpdatePathsResult::DeadEnd(chosen_path);
        }

        let mut paths_write = self.paths.write().unwrap();

        for (i, node) in sure_unvisited.into_iter().enumerate() {
            visited_write.push_back(node.clone());
            if i == 0 {
                let mut chosen_path_write = chosen_path.write().unwrap();
                chosen_path_write.push_back(node);
            } else {
                let mut new_path = chosen_path.read().unwrap().clone();
                new_path.push_back(node);
                paths_write.push_back(Arc::new(RwLock::new(new_path)));
            }
        }

        UpdatePathsResult::Updated
    }

    fn choose_path(&mut self, rand_gen: &mut ThreadRng) -> ChoosePathResult<T> {

        let paths_read = self.paths.read().unwrap();
        if paths_read.is_empty() { return ChoosePathResult::NoPaths; }

        let chosen_path = paths_read.iter().choose(rand_gen).unwrap();
        let chosen_path_read = chosen_path.read().unwrap();

        let path_last_node = chosen_path_read.last().unwrap();
        if *path_last_node == self.end_node {
            return ChoosePathResult::FinalPathFound(chosen_path.clone());
        }

        let unvisited_nodes = {
            let last_node_neighbours = &self.graph[path_last_node];
            let visited_nodes_read = self.visited_nodes.read().unwrap();
            last_node_neighbours.iter().filter(|neighbour_node| {
                !visited_nodes_read.contains(neighbour_node)
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