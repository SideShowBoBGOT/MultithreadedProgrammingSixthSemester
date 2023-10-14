use std::collections::HashMap;
use std::hash::Hash;
use lazy_static::lazy_static;
use course_work::single_bfs::RcNode;
use course_work::types::{ArcNode};

#[derive(PartialEq, Eq, Hash, Copy, Clone)]
enum Direction { Up, Down, Left, Right }

lazy_static!(
    static ref DIRECTIONS: HashMap<Direction, bool> = HashMap::from([
        (Direction::Up, true),
        (Direction::Down, true),
        (Direction::Left, true),
        (Direction::Right, true)
    ]);
);

trait NewMixin {
    fn new(value: usize) -> Self;
}

impl NewMixin for RcNode<usize> {
    fn new(value: usize) -> Self {
        RcNode::new(value)
    }
}

impl NewMixin for ArcNode<usize> {
    fn new(value: usize) -> Self {
        ArcNode::new(value)
    }
}

pub fn get_rc_grid_graph(grid_size: usize)
    -> (HashMap<RcNode<usize>, Vec<RcNode<usize>>>, Vec<RcNode<usize>>) {
    get_grid_graph::<RcNode<usize>>(grid_size)
}

pub fn get_arc_grid_graph(grid_size: usize)
    -> (HashMap<ArcNode<usize>, Vec<ArcNode<usize>>>, Vec<ArcNode<usize>>) {
    get_grid_graph::<ArcNode<usize>>(grid_size)
}

fn get_index(i: usize, j: usize, grid_size: usize) -> usize { i * grid_size + j }

fn get_grid_graph<T: NewMixin + Clone + Eq + Hash>(grid_size: usize) -> (HashMap<T, Vec<T>>, Vec<T>) {
    let grid_size_squared = grid_size * grid_size;
    let mat = (0..grid_size_squared).map(|i| T::new(i)).collect::<Vec<_>>();
    let mut graph = (0..grid_size_squared).map(|i| (mat[i].clone(), vec![])).collect::<HashMap<T, Vec<T>>>();
    let last_index = grid_size - 1;
    for i in 0..grid_size {
        for j in 0..grid_size {
            let mut dirs_c = DIRECTIONS.clone();
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
            let neighbors = graph.get_mut(&mat[get_index(i, j, grid_size)]).unwrap();
            for (d, ok) in dirs_c {
                if !ok {continue;}
                let index = match d {
                    Direction::Up => get_index(i - 1, j, grid_size),
                    Direction::Down => get_index(i + 1, j, grid_size),
                    Direction::Left => get_index(i, j - 1, grid_size),
                    Direction::Right => get_index(i, j + 1, grid_size),
                };
                neighbors.push(mat[index].clone())
            }
        }
    }
    (graph, mat)
}