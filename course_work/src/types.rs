use std::collections::HashMap;
use std::sync::{Arc, RwLock};
use std::hash::{Hash, Hasher};
use std::ops::{Deref, DerefMut};
pub struct SingleNode<T>(Arc<T>);

impl<T> SingleNode<T> {
    pub fn new(value: T) -> Self {
        Self(Arc::new(value))
    }
}

impl<T> PartialEq<Self> for SingleNode<T> {
    fn eq(&self, other: &Self) -> bool {
        Arc::ptr_eq(&self.0, &other.0)
    }
}

impl<T> Eq for SingleNode<T> {}

impl<T> Clone for SingleNode<T> {
    fn clone(&self) -> Self {
        Self(self.0.clone())
    }
}

impl<T: Hash> Hash for SingleNode<T> {
    fn hash<H: Hasher>(&self, state: &mut H) {
        self.0.hash(state)
    }
}

impl<T> Deref for SingleNode<T> {
    type Target = Arc<T>;

    fn deref(&self) -> &Self::Target { &self.0 }
}

impl<T> DerefMut for SingleNode<T> {
    fn deref_mut(&mut self) -> &mut Self::Target { &mut self.0 }
}

pub type SinglePath<T> = Arc<RwLock<im::Vector<SingleNode<T>>>>;
pub type ResultPath<T> = Arc<RwLock<Option<SinglePath<T>>>>;
pub type Paths<T> = Arc<RwLock<Vec<SinglePath<T>>>>;
pub type NodesMap<T> = Arc<RwLock<HashMap<SingleNode<T>, Vec<SingleNode<T>>>>>;
pub type Graph<T> = Arc<HashMap<SingleNode<T>, Vec<SingleNode<T>>>>;
pub type CommunicationMarker = Arc<RwLock<bool>>;