use std::collections::HashMap;
use std::sync::Arc;
use crate::my_bfs::SingleNode;
use crate::my_grid_graph::get_grid_graph;

mod my_bfs;
mod my_types;
mod my_grid_graph;

fn main() {

    let grid_size = 10;
    let (grid, mat) = get_grid_graph(grid_size);

    let last_index = grid_size - 1;
    let start = &mat[0][0];
    let end = &mat[last_index][last_index];
    let total_threads = 6;
    for i in 0..1 {
        let mut path_opt = my_bfs::find_path(
            start.clone(), end.clone(), grid.clone(), total_threads);
        let mut path = path_opt.unwrap();
        println!("Iteration: {}", i);
        println!("Path:");
        for el in path {
             println!("{}", *el);
        }
    }

}
