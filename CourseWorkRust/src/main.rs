mod atomic_queue;
mod bfs;

trait A {
    fn foo(&self);
}

trait B : A {

}
struct C {

}

impl A for C {
    fn foo(&self) {

    }
}

impl B for C {

}

fn main() {
    let c = C{};
    c.foo();
}
