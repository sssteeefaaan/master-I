use std::{collections::HashMap, fmt::Display};

#[derive(Debug)]
struct Trie<T>{
    value: Vec<T>,
    key: char,
    children: HashMap<char, Box<Trie<T>>>
}

impl<T:Display> Trie<T>{
    fn new() -> Self{
        Trie{
            value: Vec::new(),
            key: '\0',
            children: HashMap::new()
        }
    }

    fn delete(&mut self, key: String) -> Option<Vec<T>>{
        if key.len() < 1{
            return None;
        }

        let first = key.chars().nth(0).unwrap();
        match self.children.get_mut(&first){
            Some(v)=> {
                if key.len() == 2 {
                    let second = key.chars().nth(1).unwrap();
                    match v.children.remove(&second){
                        Some(ret)=>{
                            for (ck, cv) in ret.children{
                                match v.children.get_mut(&ck){
                                    Some(child)=>{
                                        child.value.extend(cv.value);
                                    },
                                    None => {
                                        v.children.insert(ck, cv);
                                    }
                                }
                            }
                            Some(ret.value)
                        },
                        None => None
                    }
                }else if key.len() > 1 {
                    v.delete(key.get(1..).unwrap().to_string())
                }else {
                    None
                }
            },
            None=> {
                None
            }
        }
    }

    fn print(&self){
        print!("{{ ");
        for el in &self.value{
            print!("{}, ", el)
        }
        println!("}}");
    }

    fn postorder(&mut self, key:&String){
        let mut new_key = key.clone();
        new_key.push_str(&self.key.to_string());

        for (_, v) in &mut self.children{
            v.postorder(&new_key);
        }

        print!("{}: ", new_key);
        self.print();
    }

    fn preorder(&mut self, key:&String){
        let mut new_key = key.clone();
        new_key.push_str(&self.key.to_string());

        print!("{}: ", new_key);
        self.print();

        for (_, v) in &mut self.children{
            v.preorder(&new_key);
        }
    }

    fn insert(&mut self, key: String, value: T)->bool{
        if key.len() < 1{
            return false;
        }

        let first = key.chars().nth(0).unwrap();
        if self.key == first && key.len() == 1{
            self.value.push(value);
            return true;
        }

        let (new_first, rest) =
        if self.key == '\0'{
            // Root
            (first, key.clone())
        }else{
            (key.chars().nth(1).unwrap(), key.get(1..).unwrap().to_string())
        };

        match self.children.get_mut(&new_first){
            Some(child)=>{
                child.insert(rest, value)
            },
            None=>{
                let mut new_child = Trie{
                    key: new_first.clone(),
                    value: Vec::new(),
                    children:HashMap::new()
                };
                let ret = new_child.insert(rest, value);
                self.children.insert(new_first, Box::new(new_child));
                ret
            }
        }
    }
}

fn main(){

    let mut t = Trie::new();

    t.insert("Probai".to_string(), 10);
    t.insert("Proba".to_string(), 11);
    t.insert("Proba".to_string(), 12);
    t.insert("Probam".to_string(), 13);
    t.insert("Probali".to_string(), 14);
    t.insert("Probacu".to_string(), 15);
    t.insert("Probno".to_string(), 16);

    println!("{:#?}", t);

    println!("Deleted: {:#?}", t.delete("Probal".to_string()));

    println!("{:#?}", t);

    println!("\n\nPREORDER\n\n");
    t.preorder(&"".to_string());

    println!("\n\nPOSTORDER\n\n");
    t.postorder(&"".to_string());
}