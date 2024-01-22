use std::collections::HashMap;
use std::ptr;

mod atomic_queue;
mod bfs;

fn main() {
    let mut m = HashMap::new();
    m.insert(1, 1);
    m.insert(2, 1);
    m.insert(3, 1);
    m.insert(4, 1);
    m.insert(5, 1);
    m.insert(6, 1);
    m.insert(7, 1);
    m.insert(8, 1);
    m.insert(9, 1);

    println!("{}", m.len());

    let addr = ptr::addr_of!(m) as usize;
    let mm = (addr as *const HashMap<i32, i32>);
    unsafe { println!("{}", (*mm).len()); }

}
