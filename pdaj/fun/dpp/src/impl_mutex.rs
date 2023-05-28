use std::{sync::{Mutex, Arc, RwLock}, thread, ops::{AddAssign, SubAssign}, io::stdin};

const PHILOSOPHER_NUMBER : usize = 5;
struct Philosopher{
    room: Arc<Mutex<usize>>,
    right_fork: Arc<Mutex<Fork>>,
    left_fork: Arc<Mutex<Fork>>,
    name: String,
    ate: usize
}

impl Philosopher{
    fn eat(&mut self){
        if self.left_fork.try_lock().is_ok() && self.right_fork.try_lock().is_ok(){
            self.ate += 1;
            println!("Philosopher {} is eating for the {} time!", self.name, self.ate);
        }else{
            println!("Philosopher {} is thinking", self.name);
        }
    }

    fn enter_room(&mut self) -> bool{
        let mut people_in_the_room = self.room.lock().unwrap();
        if people_in_the_room.lt(&PHILOSOPHER_NUMBER){
            people_in_the_room.add_assign(1);
            true
        }else{
            false
        }
    }

    fn leave_room(&mut self){
        self.room.lock().unwrap().sub_assign(1);
    }
}
struct Fork;

pub fn main(){
    let run = Arc::new(RwLock::new(true));
    let room = Arc::new(Mutex::new(0_usize));
    let mut forks = Vec::new();
    for _ in 0..PHILOSOPHER_NUMBER{
        forks.push(Arc::new(Mutex::new(Fork{})));
    }

    let mut threads = Vec::new();
    for i in 0..PHILOSOPHER_NUMBER{
        let left_fork = forks[i].clone();
        let right_fork = forks[(i + 1) % PHILOSOPHER_NUMBER].clone();
        let room_clone = room.clone();
        let run_clone = run.clone();
        threads.push(thread::spawn(move ||{
            let mut p = Philosopher{
                left_fork,
                right_fork,
                name: format!("Philosopher{}", i),
                ate: 0,
                room: room_clone
            };

            while run_clone.read().unwrap().to_owned(){
                if p.enter_room(){
                    p.eat();
                    p.leave_room();
                }
            }
        }));
    }

    let mut _forget = String::new();
    stdin().read_line(&mut _forget).expect("Error");
    (*run.as_ref().write().unwrap()) = false;

    for t in threads{
        t.join().unwrap();
    }
}