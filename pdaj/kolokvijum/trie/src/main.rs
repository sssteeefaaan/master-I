use std::collections::HashMap;


#[derive(Debug)]
struct Trie<T>{
    value: Vec<T>,
    key: char,
    children: HashMap<char, Box<Trie<T>>>
}

impl<T> Trie<T>{
    fn new() -> Self{
        Trie{
            value: Vec::new(),
            key: '\0',
            children: HashMap::new()
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

    t.insert("Proba".to_string(), 10);
    t.insert("Proba".to_string(), 11);
    t.insert("Proba".to_string(), 12);
    t.insert("Probam".to_string(), 13);
    t.insert("Probali".to_string(), 14);
    t.insert("Probacu".to_string(), 15);
    t.insert("Probno".to_string(), 16);

    println!("{:#?}", t);
}