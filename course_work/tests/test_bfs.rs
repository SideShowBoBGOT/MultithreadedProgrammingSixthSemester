pub mod grid_graph;

use course_work::single_bfs;
//use grid_graph::get_grid_graph;
#[test]
fn test_one_thread() {
    // let grid_size = 3;
    // let last_index = grid_size - 1;
    // let (grid, mat) = get_grid_graph(grid_size);
    // let start_node = mat[0][0].clone();
    // let end_node = mat[last_index][last_index].clone();
    // drop(mat);
    //
    // let start_time = std::time::SystemTime::now();
    // let path = single_bfs::find_path(&start_node, &end_node, &grid).unwrap();
    //
    // for node in path {
    //     println!("{}", *node);
    // }
    //
    // let dur = std::time::SystemTime::now().duration_since(start_time).unwrap().as_millis();
    //
    // println!("{}", dur);
}

#[test]
fn test_four_thread() {
    // let grid_size = 500;
    // let last_index = grid_size - 1;
    // let (grid, mat) = get_grid_graph(grid_size);
    // let start_node = &mat[0][0];
    // let end_node = &mat[last_index][last_index];
    //
    //
    // let start_time = std::time::SystemTime::now();
    //
    // find_path(start_node.clone(), end_node.clone(), grid.clone(), 2);
    //
    // let dur = std::time::SystemTime::now().duration_since(start_time).unwrap().as_millis();
    //
    // println!("{}", dur);
}
