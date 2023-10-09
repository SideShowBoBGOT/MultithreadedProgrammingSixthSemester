use std::sync::Arc;
use im::{HashMap, hashmap};
use crate::bfs::{SingleNode, Graph};

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

    let mut graph = Arc::new(
        hashmap! {
            a.clone() => vec![b.clone(), c.clone(), d.clone(), e.clone()],
            b.clone() => vec![f.clone()],
            c.clone() => vec![d.clone(), g.clone(), j.clone()],
            d.clone() => vec![j.clone(), g.clone(), e.clone()],
            e.clone() => vec![h.clone(), k.clone(), a.clone()],
            f.clone() => vec![c.clone(), j.clone()],
            g.clone() => vec![d.clone(), e.clone(), k.clone(), m.clone(), l.clone()],
            h.clone() => vec![e.clone(), k.clone()],
            j.clone() => vec![g.clone(), l.clone()],
            l.clone() => vec![g.clone(), m.clone()],
            m.clone() => vec![],
            k.clone() => vec![g.clone(), m.clone()]
        }
    );
    bfs::find_path(a.clone(), m.clone(), graph, 1);
}
