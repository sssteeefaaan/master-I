use std::io::{ stdin, stdout, Write };
use std::collections::HashMap;

fn fibonacci(n : i64, cache : &mut HashMap<i64, i128>) -> i128{
    if n < 2 {
        return 1;
    }
    match cache.get(&n){
        Some(&value) => value,
        None => {
            let value = fibonacci(n - 1, cache) + fibonacci(n - 2, cache);
            cache.insert(n, value);
            value
        }
    }
}

fn main(){
    let mut input = String::new();
    let n = 'main_loop : loop {
        print!("Insert how many Fibonnaci numbers you want to see: ");
        let _ = stdout().flush();
        input.clear();
        stdin()
        .read_line(&mut input)
        .expect("Wrong input!");
        match input.trim().parse::<i64>(){
            Ok(value) => 
            { break 'main_loop value; },
            Err(..) => println!("Value '{input}' isn't an integer!")
        }
    };
    let mut i = 0;
    let mut cache = HashMap::<i64, i128>::new();
    while i < n {
        println!("{i} -> {}",fibonacci(i, &mut cache));
        i += 1;
    }
}