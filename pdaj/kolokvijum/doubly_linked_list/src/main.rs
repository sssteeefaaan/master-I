// use std::borrow::BorrowMut;
// use std::rc::{Rc, Weak};
// use std::cell::RefCell;
// use std::fmt::Debug;

// type NodePtr<T> = Option<Rc<RefCell<Node<T>>>>;
// struct Node<T : Copy>{
//     value: T,
//     next: NodePtr<T>,
//     previous: Option<Weak<RefCell<Node<T>>>>
// }

// struct DoubleLinkedList<T : Copy>{
//     head: NodePtr<T>,
//     tail: NodePtr<T>,
//     current: NodePtr<T>,
//     length: u64
// }

// impl<T: Copy> Node<T>{
//     fn new(element:T)->Self{
//         Node{
//             value:element,
//             next:None,
//             previous: None
//         }
//     }
// }

// impl<T:Copy> From<Node<T>> for Option<Rc<RefCell<Node<T>>>>{
//     fn from(node: Node<T>) -> Self {
//         Some(Rc::new(RefCell::new(node)))
//     }
// }
// trait HasNew{
//     fn new()->Self;
// }

// impl <T : Debug + HasNew + Copy> DoubleLinkedList<T>{
//     fn new()->Self{
//         DoubleLinkedList { head: None, tail: None, current: None, length: 0 }
//     }

//     fn push_front(&mut self, element: T){
//         let mut new_node = Node::new(element);

//         match &mut self.head.take(){
//             Some(head)=>{
//                 new_node.next = Some(head.clone());
//                 self.head = Some(Rc::new(RefCell::new(new_node)));
//                 if let Some(h) = &self.head {
//                     head.borrow_mut().previous = Some(Rc::downgrade(&h));
//                 };
//             },
//             None=>{
//                 self.head = Some(Rc::new(RefCell::new(new_node)));
//                 self.tail = self.head.clone();
//                 self.current = self.tail.clone();
//             }
//         }

//         self.length+=1;
//     }
// }