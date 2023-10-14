pub mod grid_graph;

use std::collections::HashMap;
use std::iter::zip;
use std::rc::Rc;
use course_work::single_bfs;
use course_work::single_bfs::RcNode;
use course_work::types::{PathSize};
use crate::grid_graph::get_rc_grid_graph;
use crate::PathValidError::{EmptyPath, NotInNeighbours, StartNodeNotMatch};

#[test]
fn test_one_thread() {
    let grid_size = 30;
    let last_index = grid_size - 1;
    let (grid, mat) = get_rc_grid_graph(grid_size);
    let start_node = mat[0][0].clone();
    let end_node = mat[last_index][last_index].clone();
    drop(mat);

    let start_time = std::time::SystemTime::now();
    let path = single_bfs::find_path(&start_node, &end_node, &grid, PathSize::Unlimited).unwrap();

    let dur = std::time::SystemTime::now().duration_since(start_time).unwrap().as_millis();

    println!("Duration: {}", dur);
}

#[derive(Debug)]
enum PathValidError {
    EmptyPath,
    StartNodeNotMatch(RcNode<usize>),
    EndNodeNotMatch(RcNode<usize>),
    NotInNeighbours{parent: RcNode<usize>, child: RcNode<usize>}
}

fn check_path_valid<T>(path: &mut im::Vector<RcNode<usize>>, graph: &HashMap<RcNode<T>, Vec<RcNode<T>>>,
                       start_node: &RcNode<usize>, end_node: &RcNode<usize>) -> Result<(), PathValidError> {
    let front_node = path.front().ok_or(EmptyPath)?;
    if !Rc::ptr_eq(front_node, start_node) {
        return Err(StartNodeNotMatch(front_node.clone()));
    }
    let back_node = path.back().ok_or(EmptyPath)?;
    if !Rc::ptr_eq(back_node, end_node) {
        return Err(StartNodeNotMatch(back_node.clone()));
    }

    let path_slice = path.slice(1..);
    for (parent, child) in zip(&*path, &path_slice) {
        if !graph[parent].contains(child) {
            return Err(NotInNeighbours {parent: parent.clone(), child: child.clone()})
        }
    }

    Ok(())
}
