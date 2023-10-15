use std::collections::HashMap;
use std::sync::{Arc, RwLock};


#[derive(Clone, Copy)]
pub enum PathSize {
    Unlimited,
    Limited(usize)
}