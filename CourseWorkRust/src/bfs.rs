use std::collections::HashMap;
use std::hash::Hash;
use std::sync::atomic::{AtomicBool, Ordering};
use crate::atomic_queue::A

pub fn sequential_bfs<'a, T>(
    graph: &'a Graph<T>,
    start: &'a T,
    end: &'a T
) -> Option<Vec<&'a T>>
    where T: Eq + Hash
{
    SequentialBFS{graph, start, end}.solve()
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
    fn determine_path(
        &self,
        visitor_map: HashMap<&'a T, (AtomicBool, &'a T)>
    ) -> Vec<&'a T>
    {
        let mut path = vec![self.end];
        let mut current_node = self.end;
        while *current_node != *self.start {
            current_node = visitor_map.get(current_node).unwrap().1;
            path.push(current_node);
        }
        path.reverse();
        return path;
    }

    fn shared(&self) -> Option<Vec<&'a T>> {
        let queue = AtomicQueue

        unimplemented!();
    }

}

#[cfg(test)]
mod test {
    use std::collections::HashMap;
    use crate::bfs::sequential_bfs;

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
}






