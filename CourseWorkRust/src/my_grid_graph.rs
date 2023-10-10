use std::sync::Arc;
use std::collections::HashMap;
use crate::my_types::{Graph, SingleNode};

#[derive(PartialEq, Eq, Hash, Copy, Clone)]
enum Direction { Up, Down, Left, Right }

fn get_index(i: usize, j: usize, grid_size: usize) -> usize { i * grid_size + j }

fn get_mat(grid_size: usize) -> Vec<Vec<SingleNode<usize>>> {
    let mut grid = Vec::with_capacity(grid_size);
    for i in 0..grid_size {
        let mut row = Vec::with_capacity(grid_size);
        for j in 0..grid_size {
            row.push(SingleNode::new(get_index(i, j, grid_size)));
        }
        grid.push(row);
    }
    grid
}

pub fn get_grid_graph(grid_size: usize) -> (Graph<usize>, Vec<Vec<SingleNode<usize>>>) {
    let mat = get_mat(grid_size);
    let dirs = HashMap::from([
        (Direction::Up, true),
        (Direction::Down, true),
        (Direction::Left, true),
        (Direction::Right, true)
    ]);
    let mut graph = HashMap::new();
    for i in 0..grid_size {
        for j in 0..grid_size {
            graph.insert(mat[i][j].clone(), vec![]);
        }
    }
    let last_index = grid_size - 1;
    for i in 0..grid_size {
        for j in 0..grid_size {
            let mut dirs_c = dirs.clone();
            if i == 0 {
                dirs_c.insert(Direction::Up, false);
            } else if i == last_index {
                dirs_c.insert(Direction::Down, false);
            }
            if j == 0 {
                dirs_c.insert(Direction::Left, false);
            } else if j == last_index {
                dirs_c.insert(Direction::Right, false);
            }
            let neighbors = graph.get_mut(&mat[i][j]).unwrap();
            for (d, ok) in dirs_c {
                if !ok {continue;}
                match d {
                    Direction::Up => neighbors.push(mat[i - 1][j].clone()),
                    Direction::Down => neighbors.push(mat[i + 1][j].clone()),
                    Direction::Left => neighbors.push(mat[i][j - 1].clone()),
                    Direction::Right => neighbors.push(mat[i][j + 1].clone()),
                }
            }
        }
    }
    (Arc::new(graph), mat)
}