use std::collections::HashMap;
use std::hash::{Hash};
use im::Vector;
use rand::prelude::*;
use crate::types::{RcGraph, RcNode};

pub fn find_path<T>(start_node: &RcNode<T>, end_node: &RcNode<T>, graph: &RcGraph<T>)
    -> Option<Vector<RcNode<T>>>
    where T: Eq + Hash {
    let mut graph_visited: RcGraph<T> = HashMap::new();
    let mut paths = vec![im::Vector::from(vec![start_node.clone()])];
    let mut rand_gen = ThreadRng::default();
    'main_loop: loop {
        if paths.is_empty() { break 'main_loop None; }
        let chosen_path_index = rand_gen.gen_range(0..paths.len());
        let path_last_node = paths[chosen_path_index].last().unwrap();
        if path_last_node == end_node { break 'main_loop Some(paths[chosen_path_index].clone()); }
        let last_node_neighbours = &graph[path_last_node];
        if last_node_neighbours.is_empty() {
            paths.remove(chosen_path_index);
            continue 'main_loop;
        }
        let last_index = last_node_neighbours.len() - 1;
        if let Some(visited_nodes) = graph_visited.get_mut(path_last_node) {
            let mut is_any_unvisited = false;
            for node in &last_node_neighbours[0..last_index] {
                if visited_nodes.contains(node) {continue;}
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
        } else {
            graph_visited.insert(path_last_node.clone(), vec![]);
            let visited_nodes = graph_visited.get_mut(path_last_node).unwrap();
            for node in &last_node_neighbours[0..last_index] {
                visited_nodes.push(node.clone());
                let mut new_path = paths[chosen_path_index].clone();
                new_path.push_back(node.clone());
                paths.push(new_path);
            }
            visited_nodes.push(last_node_neighbours[last_index].clone());
            paths[chosen_path_index].push_back(last_node_neighbours[last_index].clone());
        }
    }
}

