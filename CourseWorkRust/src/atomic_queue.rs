use std::sync::atomic::{Ordering, AtomicBool};

pub struct AtomicQueue<T> {
    queue: Vec<T>,
    flag: AtomicBool
}

// It is much faster to use atomics as locks than standard Mutex
impl<T> AtomicQueue<T> {
    pub fn push(&mut self, value: T) {
        while(self.flag.fetch_or(true, Ordering::SeqCst)) {}
        self.queue.push(value);
        self.flag.fetch_and(false, Ordering::SeqCst);
    }

    pub fn pop(&mut self) -> Option<T> {
        while(self.flag.fetch_or(true, Ordering::SeqCst)) {}
        let popped = self.queue.pop();
        self.flag.fetch_and(false, Ordering::SeqCst);
        return popped;
    }
}