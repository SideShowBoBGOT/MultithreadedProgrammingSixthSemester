use std::collections::HashMap;
use std::sync::Arc;
use crate::bfs::SingleNode;

mod bfs;

fn main() {
    let a = SingleNode::new("A");
    let b = SingleNode::new("B");
    let c = SingleNode::new("C");
    let d = SingleNode::new("D");
    let e = SingleNode::new("E");
    let f = SingleNode::new("F");
    let g = SingleNode::new("G");
    let h = SingleNode::new("H");
    let j = SingleNode::new("J");
    let l = SingleNode::new("L");
    let m = SingleNode::new("M");
    let k = SingleNode::new("K");

    let mut graph = HashMap::new();
    graph.insert(a.clone(), vec![b.clone(), c.clone(), d.clone(), e.clone()]);
    graph.insert(b.clone(), vec![f.clone()]);
    graph.insert(c.clone(), vec![d.clone(), g.clone(), j.clone()]);
    graph.insert(d.clone(), vec![j.clone(), g.clone(), e.clone()]);
    graph.insert(e.clone(), vec![h.clone(), k.clone(), a.clone()]);
    graph.insert(f.clone(), vec![c.clone(), j.clone()]);
    graph.insert(g.clone(), vec![d.clone(), e.clone(), k.clone(), m.clone(), l.clone()]);
    graph.insert(h.clone(), vec![e.clone(), k.clone()]);
    graph.insert(j.clone(), vec![g.clone(), l.clone()]);
    graph.insert(l.clone(), vec![g.clone(), m.clone()]);
    graph.insert(m.clone(), vec![]);
    graph.insert(k.clone(), vec![g.clone(), m.clone()]);
    let graph = Arc::new(graph);
    let total_threads = 6;
    for i in 0..100000 {
        let mut path_opt = bfs::find_path(a.clone(), m.clone(), graph.clone(), total_threads);
        let mut path = path_opt.unwrap();
        println!("{}", i);
        // for el in path {
        //     println!("{}", *el.write().unwrap());
        // }
    }

}
