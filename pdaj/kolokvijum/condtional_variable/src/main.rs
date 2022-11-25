use std::collections::{HashMap, VecDeque};
use rand::Rng;

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

    fn wait(&mut self, thread: Thread){
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
    let mut rand = rand::thread_rng();
    let mut cond_var = ConditionalVariable::new();

    for i in 0..100{
        let mut t = Thread::new();
        t.id = i.to_string();
        t.message = "Hello World".to_string();
        t.message.push_str(t.id.as_str());
        t.priority = rand.gen_range(0..=4);
        cond_var.wait(t);
    }

    for _ in 0..=100{
        cond_var.notify_one();
    }
}