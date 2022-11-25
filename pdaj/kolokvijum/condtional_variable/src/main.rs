use std::collections::{HashMap, VecDeque};

struct Thread{
    id: String,
    message: String,
    priority: u64
}

struct ConditionalVariable{
    queue: HashMap<u64, VecDeque<Thread>>
}

impl Thread{
    fn new() -> Self{
        Thread { id: "".to_string(), message: "".to_string(), priority: 0 }
    }

    fn print(&self){
        println!("T[{}, {}]: {}", self.priority, self.id, self.message);
    }
}

impl ConditionalVariable{
    fn new() -> Self{
        ConditionalVariable{
            queue: HashMap::new()
        }
    }

    fn add_to_queue(&mut self, thread: Thread){
        match self.queue.get_mut(&thread.priority){
            Some(queue)=>{
                queue.push_back(thread);
            },
            None=>{
                let mut queue = VecDeque::new();
                let thread_priority = thread.priority;
                queue.push_back(thread);
                self.queue.insert(thread_priority, queue);
            }
        }
    }

    fn notify_one(&mut self) {
        let min_priority = self.queue.keys().min();

        if min_priority.is_none(){
            println!("Queue is empty!");
        }else{
            let min = min_priority.unwrap().clone();
            match self.queue.get_mut(&min){
                Some(v)=>{
                    if v.is_empty(){
                        self.queue.remove(&min);
                        self.notify_one();
                    }else{
                        let first_thread = v.pop_front();
                        first_thread.unwrap().print();
                    }
                },
                None => {}
            }
        }
    }
}

fn main(){
    let t1 = Thread{
        id: "1".to_string(),
        message: "Hello World!".to_string(),
        priority: 10
    };
    let t2 = Thread{
        id: "2".to_string(),
        message: "Hello beautiful World!".to_string(),
        priority: 11
    };
    let t3 = Thread{
        id: "3".to_string(),
        message: "Hello awful World!".to_string(),
        priority: 10
    };

    let mut cond_var = ConditionalVariable::new();
    cond_var.add_to_queue(t3);
    cond_var.add_to_queue(t2);
    cond_var.add_to_queue(t1);

    cond_var.notify_one();
    cond_var.notify_one();
    cond_var.notify_one();
    cond_var.notify_one();
}