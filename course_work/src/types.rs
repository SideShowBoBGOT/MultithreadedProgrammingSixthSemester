use std::collections::HashMap;
use std::sync::{Arc, RwLock};

pub type ArcNode<T> = Arc<T>;
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