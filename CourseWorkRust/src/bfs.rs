use std::collections::HashMap;
use std::hash::Hash;
use std::{mem, ptr};
use std::sync::atomic::{AtomicBool, AtomicUsize, Ordering};
use crate::atomic_queue::AtomicQueue;

pub fn sequential_bfs<'a, T>(
    graph: &'a Graph<T>,
    start: &'a T,
    end: &'a T
) -> Option<Vec<&'a T>>
    where T: Eq + Hash
{
    SequentialBFS{graph, start, end}.solve()
}

pub unsafe fn shared_bfs<'a, T>(
    graph: &'a Graph<T>,
    start: &'a T,
    end: &'a T,
    threads_num: usize
) -> Option<Vec<&'a T>>
    where T: Eq + Hash
{
    ParallelBFS{graph, start, end, threads_num}.shared()
}

type Graph<T> = HashMap<T, Vec<T>>;

struct SequentialBFS<'a, T> {
    graph: &'a Graph<T>,
    start: &'a T,
    end: &'a T
}

impl<'a, T: Eq + Hash> SequentialBFS<'a, T> {

    fn determine_path(
        &self,
        visitor_map: &HashMap<&'a T, Option::<&'a T>>
    ) -> Vec<&'a T>
    {
        let mut path = vec![self.end];
        let mut current_node = self.end;
        while *current_node != *self.start {
            current_node = visitor_map.get(current_node).unwrap().unwrap();
            path.push(current_node);
        }
        path.reverse();
        return path;
    }

    pub fn solve(&self) -> Option<Vec<&'a T>>
        where T: Eq
    {
        let mut queue = vec![self.start];
        let mut visitor_map: HashMap<&T, Option::<&T>> = self.graph.iter().map(|(k, _)| (k, None)).collect();
        while let Some(current_node) = queue.pop() {
            for neighbour in self.graph.get(current_node).unwrap() {
                let mut visitor = visitor_map.get_mut(neighbour).unwrap();
                if visitor.is_some() { continue; }
                *visitor = Some(current_node);
                if *neighbour == *self.end {
                    return Some(self.determine_path(&visitor_map));
                }
                queue.push(neighbour);
            }
        }
        return None;
    }
}

struct ParallelBFS<'a, T> {
    graph: &'a Graph<T>,
    start: &'a T,
    end: &'a T,
    threads_num: usize
}

impl<'a, T: Eq + Hash> ParallelBFS<'a, T> {
    unsafe fn determine_path(
        &self,
        visitor_map: &HashMap<&'a T, (AtomicBool, *const T)>
    ) -> Vec<&'a T>
    {
        let mut path = vec![self.end];
        let mut current_node = self.end;
        while *current_node != *self.start {
            current_node = &*visitor_map.get(current_node).unwrap().1;
            path.push(current_node);
        }
        path.reverse();
        return path;
    }

    fn create_visitor_map(&self) -> HashMap<&'a T, (AtomicBool, *const T)> {
        self.graph.iter().map(|(k, _)| (k, (AtomicBool::default(), ptr::null()))).collect()
    }

    unsafe fn shared(&self) -> Option<Vec<&'a T>> {
        // this is unsafe, for christ, do not do the same in real world apps
        let mut queue = AtomicQueue::default();
        queue.push(self.start);
        let queue_address = ptr::addr_of!(queue) as usize;

        let mut visitor_map = self.create_visitor_map();
        let visitor_map_address = ptr::addr_of!(visitor_map) as usize;

        let end_address = ptr::addr_of!(self.end) as usize;
        let graph_address = ptr::addr_of!(self.graph) as usize;

        let is_end_node_found = AtomicBool::default();
        let total_enqueued_num = AtomicUsize::default();

        std::thread::scope(|s| {
            for _ in 0..self.threads_num {
                s.spawn(|| {
                    let queue = &mut *(queue_address as *mut AtomicQueue<&T>);
                    let visitor_map = &mut *(visitor_map_address as *mut HashMap<&T, (AtomicBool, *const T)>);
                    let end = *(end_address as *const &T);
                    let graph = *(graph_address as *const &Graph<T>);
                    while !is_end_node_found.fetch_or(false, Ordering::SeqCst) {
                        if total_enqueued_num.fetch_or(0, Ordering::SeqCst) >= graph.len() {
                            return;
                        }
                        let current_node = queue.pop();
                        if current_node.is_none() { continue }
                        let current_node = current_node.unwrap();
                        for neighbour in graph.get(current_node).unwrap() {
                            let mut visitor = visitor_map.get_mut(neighbour).unwrap();
                            if visitor.0.fetch_or(true, Ordering::SeqCst) { continue; }
                            visitor.1 = current_node as *const T;
                            if *neighbour == *end {
                                is_end_node_found.fetch_or(true, Ordering::SeqCst);
                                return;
                            }
                            queue.push(neighbour);
                        }
                    }
                });
            }
        });
        if is_end_node_found.fetch_or(false, Ordering::SeqCst) {
            Some(self.determine_path(&visitor_map))
        } else {
            None
        }
    }

}

#[cfg(test)]
mod test {
    use std::collections::HashMap;
    use std::ptr;
    use crate::bfs::{sequential_bfs, shared_bfs};

    fn generate_grid(size: usize) -> HashMap<usize, Vec<usize>> {
        let mut grid: HashMap<usize, Vec<usize>> = HashMap::new();
        let total_size = size * size;
        grid.reserve(total_size);
        for index in 0..total_size {
            let x = (index % size) as isize;
            let y = (index / size) as isize;
            let outmost = (size as isize) - 1;
            let mut neighbour_indexes: Vec<usize> = Vec::new();
            for delta_y in -1..=1 {
                let new_y = y + delta_y;
                if new_y < 0 || new_y > outmost { continue; }
                let base = (new_y as usize) * size;
                for delta_x in -1..=1 {
                    if delta_y == 0 && delta_x == 0 { continue; }
                    let new_x = x + delta_x;
                    if new_x < 0 || new_x > outmost { continue; }
                    neighbour_indexes.push(base + (new_x) as usize);
                }
            }
            grid.insert(index, neighbour_indexes);
        }
        return grid;
    }

    fn last_index(size: usize) -> usize {
        return (size - 1) * size + size - 1;
    }

    #[test]
    fn test_sequential() {
        let size = 20;
        let mut graph = generate_grid(size);
        let start = 0;
        let end = last_index(size);
        let path = sequential_bfs(&graph, &start, &end);
        println!("{:?}", path);
    }

    #[test]
    fn test_shared() {
        let size = 20;
        let mut graph = generate_grid(size);
        let start = 0;
        let end = last_index(size);
        let path = unsafe { shared_bfs(&graph, &start, &end, 6) };
        println!("{:?}", path);
    }
}






