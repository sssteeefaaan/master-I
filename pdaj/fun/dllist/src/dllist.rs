use std::{fmt::Display, rc::{Rc, Weak}, sync::RwLock};

#[derive(Debug)]
pub struct DLList<T> where T: Clone {
    pub head: Option<Rc<RwLock<Node<T>>>>,
    pub tail: Option<Weak<RwLock<Node<T>>>>,
    pub count: usize
}

#[derive(Debug)]
pub struct Node<T> where T:Clone{
    pub value: T,
    pub next: Option<Rc<RwLock<Node<T>>>>,
    pub previous: Option<Weak<RwLock<Node<T>>>>,
}

impl<T : Display + Clone> Display for Node<T>{
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        todo!()
    }
}

impl<T : Default + Clone> Default for Node<T>{
    fn default() -> Self {
        Node { value: T::default(), previous: None, next: None }
    }
}

impl<T : Default + Clone> Default for DLList<T>{
    fn default() -> Self {
        DLList { head : None, tail : None, count: 0 }
    }
}

impl<T : Default + Clone> DLList<T>{
    fn push_front(&mut self, value: T){
        match self.count{
            0 => {
                self.head = Some(Rc::new(RwLock::new(Node{value, ..Node::default()})));
                self.tail = Some(Rc::downgrade(self.head.as_ref().unwrap()));
            }
            1 => {
                self.head = Some(Rc::new(RwLock::new(Node{value, previous: None, next: self.head.take()})));
                self.tail.as_ref().unwrap().upgrade().unwrap().write().as_mut().unwrap().previous =
                Some(Rc::downgrade(self.head.as_ref().unwrap()));
            }
            _ => {
                self.head = Some(Rc::new(RwLock::new(Node{value, previous: None, next: self.head.take()})));
                self.head.as_ref().unwrap().read().unwrap().next.as_ref().unwrap().write().unwrap().previous =
                Some(Rc::downgrade(self.head.as_ref().unwrap()));
            }
        }
        self.count += 1;
    }

    fn push_back(&mut self, value: T){
        match self.count{
            0 => {
                self.head = Some(Rc::new(RwLock::new(Node{value, ..Node::default()})));
                self.tail = Some(Rc::downgrade(self.head.as_ref().unwrap()));
            }
            1 => {
                self.head.as_ref().unwrap().write().unwrap().next = Some(Rc::new(RwLock::new(Node{value, previous: Some(Rc::downgrade(self.head.as_ref().unwrap())), next: None})));
                self.tail = Some(Rc::downgrade(self.head.as_ref().unwrap().read().unwrap().next.as_ref().unwrap()));
            }
            _ => {
                let new_node = Some(Rc::new(RwLock::new(Node{value, next : None, previous: self.tail.take() })));
                self.tail = Some(Rc::downgrade(new_node.as_ref().unwrap()));
                self.tail
                .as_ref()
                .unwrap()
                .upgrade()
                .unwrap()
                .write()
                .unwrap()
                .previous
                .as_ref()
                .unwrap()
                .upgrade()
                .unwrap()
                .write()
                .unwrap().next = new_node;
            }
        }
        self.count += 1;
    }

    fn pop_front(&mut self) -> Option<T>{
        match self.count{
            0 => None,
            1 => {
                self.count -= 1;
                let ret = self.head.take();
                self.tail = None;
                Some(ret.unwrap().write().unwrap().value.to_owned())
            },
            2 => {
                self.count -= 1;
                let last = self.head.as_ref().unwrap().write().unwrap().next.take();
                let ret = self.head.replace(last.unwrap());
                self.head.as_ref().unwrap().write().unwrap().previous = None;
                Some(ret.unwrap().read().unwrap().value.to_owned())
            },
            _ => {
                self.count -= 1;
                let head = self.head.take();
                self.head = head.as_ref().unwrap().write().unwrap().next.to_owned().take();
                self.head.as_ref().unwrap().write().unwrap().previous = None;
                Some(head.unwrap().read().unwrap().value.to_owned())
            }
        }
    }

    fn pop_back(&mut self) -> Option<T>{
        match self.count{
            0 => None,
            1 => {
                self.count -= 1;
                let ret = self.head.take();
                self.tail = None;
                Some(ret.unwrap().write().unwrap().value.to_owned())
            },
            2 => {
                self.count -= 1;
                let ret = self.tail.replace(Rc::downgrade(self.head.as_ref().unwrap())).unwrap().upgrade().unwrap().write().unwrap().value.to_owned();
                self.head.as_ref().unwrap().write().unwrap().next = None;
                Some(ret)
            },
            _ => {
                self.count -= 1;
                let previous = self.tail.as_ref().unwrap().upgrade().unwrap().write().unwrap().previous.take();
                let ret = previous.as_ref().unwrap().upgrade().unwrap().write().unwrap().next.take();
                self.tail = previous;
                Some(ret.unwrap().read().unwrap().value.to_owned())
            }
        }
    }
}

pub fn main(){
    let mut list = DLList::default();

    list.push_back("Hello");
    list.push_back("Hi");
    list.push_back("Aloha");

    println!("{:#?}", list);

    println!("{:?}", list.pop_back());
    println!("{:?}", list.pop_back());
    println!("{:?}", list.pop_back());

    list.push_front("1");
    list.push_front("2");
    list.push_front("3");

    println!("{:#?}", list);

    println!("{:?}", list.pop_front());
    println!("{:?}", list.pop_front());
    println!("{:?}", list.pop_front());
}