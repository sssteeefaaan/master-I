use std::fmt::Debug;
#[derive(Debug)]
struct Node<T>{
    value: T,
    next: Option<Box<Node<T>>>
}

#[derive(Debug)]
struct List<T>{
    head: Option<Box<Node<T>>>,
    length: u64
}


impl<T: Debug + Eq> List<T>{
    fn new()->Self{
        List { head: None, length: 0 }
    }


    fn push_front(&mut self, element: T){
        self.head = Some(Box::new(Node{
            value: element,
            next: self.head.take()
        }));
        self.length += 1;
    }

    fn pop_front(&mut self)-> Option<T>{
       if self.head.is_none(){
            None
       }else{
        let ret = self.head.take().unwrap();
        self.head = ret.next;
        self.length -= 1;
        Some(ret.value)
       }
    }

    fn pop_back(&mut self)->Option<T>{
        if self.head.is_none(){
            return None
        }else if self.head.as_ref().unwrap().next.is_none(){
            return self.pop_front();
        }else{
            let mut temp = &mut self.head;
            loop{
                match temp{
                    Some(v)=>{
                        if v.next.as_ref().unwrap().next.is_none(){
                            let ret = v.next.take().unwrap();
                            return Some(ret.value);
                        }else{
                            temp = &mut v.next;
                        }
                    },
                    None=>{
                        return None
                    }
                }
            }
        }
    }

    fn push_back(&mut self, element:T){
        let mut temp = &mut self.head;
        loop{
            match temp{
                Some(v)=>{
                    if v.next.is_none(){
                        v.next = Some(Box::new(Node{value:element, next:None}));
                        self.length += 1;
                        return
                    }else{
                        temp = &mut v.next;
                    }
                },
                None=>{
                    self.head = Some(Box::new(Node{
                        value: element,
                        next: temp.take()
                    }));
                    self.length += 1;
                    return;
                }
            }
        }
    }

    fn delete(&mut self, element: T) -> bool{
        if self.head.is_none(){
            return false;
        }else if self.head.as_ref().unwrap().value == element{
            self.head = self.head.take().unwrap().next;
            self.length -= 1;
            return true
        }

        let mut temp: &mut Option<Box<Node<T>>> = &mut self.head;
        loop{
            match temp{
                Some(v)=>{
                    if v.next.is_some() && v.next.as_ref().unwrap().value == element{
                        v.next = v.next.take().unwrap().next;
                        self.length -= 1;
                        return true
                    }else{
                        temp = &mut v.next;
                    }
                },
                None=>{
                    return false
                }
            }
        }
    }

    fn print(&self){
        let mut temp = self.head.as_ref();
        let mut output = String::new();
        loop{
            if temp.is_none(){
                if output.is_empty(){
                    println!("The list is empty!")
                }else{
                    println!("{}", output);
                }
                return
            }else{
                output.push_str(&format!("{:#?} ", temp.as_ref().unwrap().value));
                temp = temp.as_ref().unwrap().next.as_ref();
            }
        }
    }

    fn size_of(&self)->u64{
        self.length
    }
}

fn main(){
    let mut l = List::new();

    l.push_front("Hello".to_string());
    l.push_front("Testing...".to_string());
    l.push_back("World!".to_string());
    l.push_back("123!".to_string());

    l.print();

    let delete = "Testing...".to_string();
    println!("Deleting {} from list: {}", delete, l.delete(delete.clone()));

    l.print();

    println!("Size of the list is {}!", l.size_of());

    println!("Popped front {:?}", l.pop_front());
    println!("Popped back {:?}", l.pop_back());
    println!("Popped back {:?}", l.pop_back());

    l.print();
}