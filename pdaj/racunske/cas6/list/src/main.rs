use std::fmt::Debug;
use std::marker::Copy;

#[derive(Clone, Debug)]
struct Node<T>{
    value: T,
    next: Option<Box<Node<T>>>,
}

struct List<T>{
    head: Option<Box<Node<T>>>,
    length: u32
}

impl<T:Debug + Copy + PartialEq> List<T>{

    pub fn new()->Self{
        List{
            head: Option::None,
            length: 0
        }
    }

    // pub fn delete(&mut self, element: &T) {
    //     let mut curr = self.head.take();
    //     let mut prev: Option<Box<Node<T>>> = None;
    //     loop{
    //         match curr{
    //             Some(v)=>{
    //                 match prev
    //                 if v.value == *element{
    //                     prev.unwrap().next = v.next;
    //                     self.length-=1;
    //                     return
    //                 }else{
    //                     prev = ;
    //                     curr = v.next;
    //                 }
    //             },
    //             _=>()
    //         }
    //     }
    // }

    pub fn append(&mut self, element:T){
        let new_node = Option::Some(Box::new(Node{value:element, next:Option::None}));

        self.length += 1;

        if self.head.is_none(){
            self.head = new_node;
        }else{
            let mut parent = &mut self.head;
            loop{
                match parent{
                    Some(v)=>{
                        if v.next.is_none(){
                            v.next = new_node;
                            return
                        }else{
                            parent = &mut v.next;
                        }
                    },
                    _=>()
                }
            }
        }
    }

    pub fn pop(&mut self) -> Option<T>{
        if self.length < 1{
            return None
        }else{
            let head = self.head.take().unwrap();
            self.head = head.next;
            self.length -= 1;
            return Some(head.value);
        }
    }

    pub fn size_of(&self) -> u32{
        self.length
    }

    pub fn print(&mut self){
        let mut next = &self.head;
        loop{
            match next{
                Some(v)=> {
                    print!("{:#?} ", v.value);
                    next = &v.next;
                },
                None=>{ println!(); return; }
            }
        }
    }
}


fn main(){
    let mut l = List::new();

    l.append("H");
    l.append("e");
    l.append("l");
    l.append("l");
    l.append("o");
    l.append("!");

    println!("First print!");
    l.print();

    println!("Popped: {}", l.pop().unwrap());

    println!("Second print!");
    l.print();

    println!("Popped: {}", l.pop().unwrap());

    println!("Third print!");
    l.print();
}