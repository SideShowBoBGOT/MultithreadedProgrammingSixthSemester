use std::collections::HashMap;
use std::sync::{Arc, RwLock};
use std::hash::{Hash, Hasher};
use std::ops::{Deref, DerefMut};
use std::rc::Rc;

pub trait Node<T: Hash + Eq + Clone> : Clone + Eq + Hash + DerefMut {
    fn new(value: T) -> Self;
}

pub struct RcNode<T>(Rc<T>);

impl<T> PartialEq<Self> for RcNode<T> {
    fn eq(&self, other: &Self) -> bool { Rc::ptr_eq(&self.0, &other.0) }
}

impl<T> Eq for RcNode<T> {}

impl<T: Hash> Hash for RcNode<T> {
    fn hash<H: Hasher>(&self, state: &mut H) { self.0.hash(state) }
}

impl<T: Hash + Eq + Clone> Node<T> for RcNode<T> {
    fn new(value: T) -> Self {
        Self(Rc::new(value))
    }
}

impl<T> Clone for RcNode<T> {
    fn clone(&self) -> Self {
        Self(self.0.clone())
    }
}

impl<T> Deref for RcNode<T> {
    type Target = Rc<T>;

    fn deref(&self) -> &Self::Target { &self.0 }
}

impl<T> DerefMut for RcNode<T> {
    fn deref_mut(&mut self) -> &mut Self::Target { &mut self.0 }
}

pub type RcGraph<T> = HashMap<RcNode<T>, Vec<RcNode<T>>>;

pub struct ArcNode<T>(Arc<T>);

impl<T> PartialEq<Self> for ArcNode<T> {
    fn eq(&self, other: &Self) -> bool {
        Arc::ptr_eq(&self.0, &other.0)
    }
}

impl<T> Eq for ArcNode<T> {}

impl<T: Hash> Hash for ArcNode<T> {
    fn hash<H: Hasher>(&self, state: &mut H) {
        self.0.hash(state)
    }
}

impl<T> Clone for ArcNode<T> {
    fn clone(&self) -> Self {
        Self(self.0.clone())
    }
}

impl<T> Deref for ArcNode<T> {
    type Target = Arc<T>;

    fn deref(&self) -> &Self::Target { &self.0 }
}

impl<T> DerefMut for ArcNode<T> {
    fn deref_mut(&mut self) -> &mut Self::Target { &mut self.0 }
}

impl<T: Hash + Eq + Clone> Node<T> for ArcNode<T> {
    fn new(value: T) -> Self {
        Self(Arc::new(value))
    }
}

pub type SinglePath<T> = Arc<RwLock<im::Vector<ArcNode<T>>>>;
pub type ResultPath<T> = Arc<RwLock<Option<SinglePath<T>>>>;
pub type Paths<T> = Arc<RwLock<Vec<SinglePath<T>>>>;
pub type NodesMap<T> = Arc<RwLock<HashMap<ArcNode<T>, Vec<ArcNode<T>>>>>;
pub type ArcGraph<T> = Arc<HashMap<ArcNode<T>, Vec<ArcNode<T>>>>;
pub type CommunicationMarker = Arc<RwLock<bool>>;
pub enum PathSize {
    Unlimited,
    Limited(usize)
}