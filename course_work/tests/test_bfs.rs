pub mod grid_graph;

use std::collections::HashMap;
use std::iter::zip;
use std::rc::Rc;
use course_work::single_bfs;
use course_work::single_bfs::RcNode;
use course_work::types::{PathSize};
use crate::grid_graph::get_rc_grid_graph;
use crate::PathValidError::{EmptyPath, NotInNeighbours, StartNodeNotMatch};
use rand::prelude::*;

#[test]
fn check_single_bfs_validity() {
    let mut rand_gen = rand::rngs::StdRng::seed_from_u64(0);
    let grid_size = 20;
    let (grid, mat) = get_rc_grid_graph(grid_size);

//    let start_time = std::time::SystemTime::now();

    for _ in 0..1000 {
        let start_end = mat.iter().choose_multiple(&mut rand_gen, 2);
        let (start_node, end_node) = (start_end[0], start_end[1]);
        let path = single_bfs::find_path(start_node, end_node, &grid, PathSize::Unlimited).unwrap();
        assert_eq!(check_valid(&path, &grid, start_node, end_node), Ok(()));
    }
  //  let dur = std::time::SystemTime::now().duration_since(start_time).unwrap().as_millis();
    //println!("Duration: {}", dur);
}

#[test]
fn check_mul_bfs_validity() {

}


#[derive(Debug, PartialEq)]
enum PathValidError {
    EmptyPath,
    StartNodeNotMatch(RcNode<usize>),
    EndNodeNotMatch(RcNode<usize>),
    NotInNeighbours{parent: RcNode<usize>, child: RcNode<usize>}
}

fn check_valid(path: &im::Vector<RcNode<usize>>, graph: &HashMap<RcNode<usize>, Vec<RcNode<usize>>>,
                       start_node: &RcNode<usize>, end_node: &RcNode<usize>) -> Result<(), PathValidError> {
    let front_node = path.front().ok_or(EmptyPath)?;
    if !Rc::ptr_eq(front_node, start_node) {
        return Err(StartNodeNotMatch(front_node.clone()));
    }
    let back_node = path.back().ok_or(EmptyPath)?;
    if !Rc::ptr_eq(back_node, end_node) {
        return Err(StartNodeNotMatch(back_node.clone()));
    }

    let path_slice = path.skip(1);
    for (parent, child) in zip(&*path, &path_slice) {
        let parent_neighbours = graph.get(parent).unwrap();
        if !parent_neighbours.contains(child) {
            return Err(NotInNeighbours {parent: parent.clone(), child: child.clone()})
        }
    }

    Ok(())
}
