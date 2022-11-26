use std::fmt::Debug;
use std::fmt::Display;

struct Transaction{
    id: String,
    sender: String,
    receiver: String,
    amount: f64
}

impl Transaction{

}

impl HasNew for Transaction{
    fn new()->Self{
        Self{
            id: String::new(),
            sender: String::new(),
            receiver: String::new(),
            amount: 0.0
        }
    }
}

impl Display for Transaction{
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        f.write_fmt(format_args!("| \t {} \t | \t {} \t | \t {} \t | \t {} \t |", self.id, self.sender, self.receiver, self.amount))
    }
}

impl PartialOrd for Transaction{
    fn partial_cmp(&self, other: &Self) -> Option<std::cmp::Ordering> {
        self.amount.partial_cmp(&other.amount)
    }
}

impl PartialEq for Transaction{
    fn eq(&self, other: &Self) -> bool {
        self.id == other.id
    }
}

#[derive(Debug)]
struct Node<T>{
    value: T,
    next: Option<Box<Node<T>>>
}

trait HasNew{
    fn new()->Self;
}
struct List<T>{
    head: Option<Box<Node<T>>>,
    length: u64
}

impl<T : HasNew + Display> Node<T>{
    fn new()->Self{
        Self{
            value: T::new(),
            next: None
        }
    }

    fn print(&self) {
        println!("{}", self.value);
    }
}

impl<T : PartialOrd + PartialEq + HasNew + Display> List<T>{

    fn new() -> Self{
        List{
            head: None,
            length: 0
        }
    }

    fn append(&mut self, element: T){
        let mut new_element = Box::new(Node { value: element, next: None });
        self.length += 1;

        if self.head.is_none(){
            self.head = Some(new_element);
        }else if self.head.as_ref().unwrap().value.ge(&new_element.value) {
            new_element.next = self.head.take();
            self.head = Some(new_element);
        }else{
            let mut temp = &mut self.head;
            loop{
                match temp{
                    Some(v)=>{
                        if v.next.is_none() || v.next.as_ref().unwrap().value > new_element.value{
                            new_element.next = v.next.take();
                            v.next = Some(new_element);
                            return;
                        }else {
                            temp = &mut v.next;
                        }
                    },
                    None=>{}
                }
            }
        }
    }

    fn pop(&mut self)->Option<T>{
        if self.head.is_none(){
            return None
        }

        let r = self.head.take().unwrap();
        self.head = r.next;

        Some(r.value)
    }

    fn delete(&mut self, element: &T) -> Option<T>{
        if self.head.is_none(){
            return None;
        }else if self.head.as_ref().unwrap().value == *element{
            return self.pop();
        }

        let mut temp = &mut self.head;
        loop{
            match temp{
                Some(v)=>{
                    if v.next.is_some() && v.next.as_ref().unwrap().value == *element{
                        let ret = v.next.take().unwrap();
                        v.next = ret.next;
                        return Some(ret.value);
                    }else{
                        temp = &mut v.next;
                    }
                },
                None => {
                    return None;
                }
            }
        }
    }

    fn print(&self){
        println!("|\t ID trans \t | \t Posiljalac \t | \t Primalac \t | \t Iznos \t |");
        let mut temp = &self.head;
        loop{
            match temp {
                Some(v)=> {
                    v.print();
                    temp = &v.next;
                },
                None => return
            }
        }
    }

}

fn main(){

    let mut l = List::new();
    let for_deletion = Transaction{id:String::from("IT 55"), sender: String::new(), receiver: String::new(), amount: 0.0};

    for i in 0..=100{
        let t = Transaction{
            id: format_args!("IT {}", i).to_string(),
            sender: i.to_string(),
            receiver: (i + 1).to_string(),
            amount: f64::from(i + 3)
        };

        l.append(t);
    }

    l.print();

    println!("After delete!");

    l.delete(&for_deletion);

    l.print();
}