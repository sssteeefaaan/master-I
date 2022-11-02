use std::collections::HashMap;

fn factorial(n : u128, cache : &mut HashMap<u128, u128>) -> u128{
    if n < 2 {
        return 1;
    }
    match cache.get(&n){
        Some(&value) => value,
        None =>{
            let value = n * factorial(n - 1, cache);
            cache.insert(n, value);
            value
        }
    }
}

fn input() -> u128 {
    use std::io::{stdin, stdout, Write};
    let mut s = String::new();
    loop{
        print!("Insert a whole number in range (0, {}): ", 32);
        let _ = stdout().flush();
        s.clear();
        stdin()
        .read_line(&mut s)
        .expect("Wrong input!");
        match s.trim().parse::<u128>(){
            Ok(value) if value < 33 => return value,
            _ => println!("Wrong input!")
        }
    }
}

fn main(){
    let n = input();
    let mut cache = HashMap::<u128, u128>::new();
    println!("{}! = {}", n, factorial(n, &mut cache));
}