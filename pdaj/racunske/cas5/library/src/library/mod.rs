pub mod author;
pub mod book;
pub mod user;
pub mod genre;

use std::collections::HashMap;
use user::User;
use book::Book;
use genre::Genre;
use author::Author;

use crate::utilities;

#[derive(Debug)]
pub struct Library{
    pub name: String,
    pub users: HashMap<String, User>,
    pub books: HashMap<String, Book>,
    pub authors: HashMap<String, Author>,
    pub genres: HashMap<String, Genre>
}

impl Library{
    pub fn create(name:String) -> Self{
        Library { name, users: HashMap::new(), books: HashMap::new(), authors: HashMap::new(), genres: HashMap::new() }
    }

    pub fn display(&self){
        println!("name: {}", self.name);
        println!("{}\nusers: ", ["+";20].join("+"));
        for (_, u) in &self.users{
            u.display();
        }
        println!("{}\nbooks: ", ["+";20].join("+"));
        for (_, b) in &self.books{
            b.display();
        }
        println!("{}\nauthors: ", ["+";20].join("+"));
        for (_, a) in &self.authors{
            a.display();
        }
        println!("{}\ngenres: ", ["+";20].join("+"));
        for (_, g) in &self.genres{
            g.display();
        }
    }
}

pub fn create_menu(libraries : &mut HashMap<String, Library>){
    let mut message = ["="; 20].join("=");
    message.push_str("\nCreate new library\n");

    loop{
        message.push_str("\nInput library name: ");
        let name = utilities::read_input(message.as_str());
        match libraries.get(&name){
            Some(_)=>{
                println!("Library with the name '{name}' already exist!");
            },
            None => {
                let lib = Library::create(name.clone());
                println!("Added new library!");
                lib.display();
                libraries.insert(name, lib);
                break;
            }
        }
        message.clear();
    }
}

pub fn display_all_libraries(libraries:&HashMap<String, Library>){
    println!("{}", ["=";20].join("="));
    if libraries.len() > 0 {
        for (_, l) in libraries{
            println!("{}", ["-";20].join("-"));
            l.display();
        }
    }
    else{
        println!("No libraries!")
    }
}

pub fn display_library(libraries:&HashMap<String, Library>){
    let mut message = ["=";20].join("=");
    message.push_str("\nDisplay library\n\n");
    loop{
        message.push_str("Input library name or 'BREAK' to exit: ");
        let name = utilities::read_input(message.as_str());
        match libraries.get(&name){
            Some(lib)=>{
                lib.display();
            },
            None=>{
                if name.to_lowercase().eq("break"){
                    return;
                }
                println!("Library with the name {name} doesn't exist!");
            }
        }
        message.clear();
    }

}