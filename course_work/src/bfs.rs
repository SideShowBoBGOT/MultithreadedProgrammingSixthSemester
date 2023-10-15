use std::collections::HashMap;
use std::hash::{Hash};
use std::rc::Rc;
use std::sync::{Arc, RwLock};
use im::Vector;
use rand::prelude::*;
use crate::types::{PathSize};

pub type RcNode<T> = Rc<T>;
pub type ArcNode<T> = Arc<T>;
pub type RcGraph<T> = HashMap<RcNode<T>, Vec<RcNode<T>>>;
pub type ArcGraph<T> = HashMap<ArcNode<T>, Vec<ArcNode<T>>>;
type CommMark = Arc<RwLock<bool>>;
type ResultPath<T> = Arc<RwLock<Option<Vector<ArcNode<T>>>>>;

trait GenericRefCounter {
    fn ptr_eq(first: &Self, second: &Self) -> bool;
}

impl<T> GenericRefCounter for RcNode<T> {
    fn ptr_eq(first: &Self, second: &Self) -> bool {
        Rc::ptr_eq(first, second)
    }
}

impl<T> GenericRefCounter for ArcNode<T> {
    fn ptr_eq(first: &Self, second: &Self) -> bool {
        Arc::ptr_eq(first, second)
    }
}

fn gen_bfs<T>(start_node: &T, end_node: &T, graph: &HashMap<T, Vec<T>>,
              paths_size: PathSize, loop_closure: &dyn Fn() -> bool) -> Option<Vector<T>>
    where T: GenericRefCounter + Eq + Hash + Clone {

    if let PathSize::Limited(limit) = paths_size {
        if limit == 0 { return None; }
    }
    let mut graph_visited: HashMap<T, Vec<T>> = HashMap::new();
    let mut paths = vec![im::Vector::from(vec![start_node.clone()])];
    let mut rand_gen = ThreadRng::default();
    'main_loop: loop {
        if !loop_closure() || paths.is_empty() { break 'main_loop None; }
        let chosen_path_index = rand_gen.gen_range(0..paths.len());
        let path_last_node = paths[chosen_path_index].last().unwrap();
        if GenericRefCounter::ptr_eq(path_last_node, end_node) { break 'main_loop Some(paths[chosen_path_index].clone()); }
        let last_node_neighbours = &graph[path_last_node];
        if last_node_neighbours.is_empty() {
            paths.remove(chosen_path_index);
            continue 'main_loop;
        }
        let last_index = last_node_neighbours.len() - 1;
        let is_contained_key = graph_visited.contains_key(path_last_node);
        if !is_contained_key { graph_visited.insert(path_last_node.clone(), vec![]); }
        let visited_nodes = graph_visited.get_mut(path_last_node).unwrap();
        let mut is_any_unvisited = false;
        'sub_loop: for node in &last_node_neighbours[0..last_index] {
            if let PathSize::Limited(limit) = paths_size {
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
    }
}

pub fn bfs<T>(start_node: &RcNode<T>, end_node: &RcNode<T>, graph: &RcGraph<T>, paths_size: PathSize)
              -> Option<Vector<RcNode<T>>>
    where T: Eq + Hash {
    gen_bfs(start_node, end_node, graph, paths_size, &|| true)
}

pub fn bfs_mul<T>(start_node: &ArcNode<T>, end_node: &ArcNode<T>, graph: &ArcGraph<T>, paths_size: PathSize, threads_num: usize)
              -> Option<Vector<ArcNode<T>>>
    where T: Eq + Hash + Send + Sync {
    let result_path: ResultPath<T> = Arc::new(RwLock::new(None));
    std::thread::scope(|s| {
        let comm_mark: CommMark = Arc::new(RwLock::new(true));
        for index in 0..threads_num {
            if index == threads_num - 1 {
                bfs_mul_impl(start_node, end_node, graph, paths_size, &comm_mark, &result_path);
                continue;
            }
            let comm_mark_clone = comm_mark.clone();
            let result_path_clone = result_path.clone();
            s.spawn(move || {
                bfs_mul_impl(start_node, end_node, graph, paths_size,
                             &comm_mark_clone, &result_path_clone)
            });
        }
    });

    let result = std::mem::take(&mut *result_path.write().unwrap());
    result
}

fn bfs_mul_impl<T: Eq + Hash>(start_node: &ArcNode<T>, end_node: &ArcNode<T>, graph: &ArcGraph<T>,
                   paths_size: PathSize, comm_mark: &CommMark, result_path: &ResultPath<T>) {
    let path = gen_bfs(start_node, end_node, graph, paths_size,
        &|| *(comm_mark.read().unwrap()) );
    let mut comm_mark_write = comm_mark.write().unwrap();
    if !*comm_mark_write {return;}
    *comm_mark_write = false;
    let mut res_write = result_path.write().unwrap();
    *res_write = path;
}



