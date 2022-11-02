fn is_prime(n : u128) -> bool{
    match n {
        0 | 1 => false,
        2 | 3 => true,
        _ => {
            if n % 2 == 0 || n % 3 == 0 {
                return false;
            }

            let mut i = 5;
            while i * i <= n {
                if n % i == 0 || n % (i + 2) == 0{
                    return false
                }
                i += 6;
            }
            true
        }
    }
}

fn input(input_directions: &str) -> u128 {
    use std::io::{stdin, stdout, Write};
    let mut s = String::new();
    loop{
        print!("{input_directions}");
        let _ = stdout().flush();
        s.clear();
        stdin()
        .read_line(&mut s)
        .expect("Wrong input!");
        match  s.trim().parse::<u128>(){
            Ok(value) => return value,
            _ => println!("Input wasn't an integer!")
        }
    }
}

fn input_range() -> (u128, u128){
    let (mut lower, mut upper) : (u128, u128);

    loop{
        (lower, upper) = (input("Input lower boundry: "), input("Input upper boundry: "));
        if lower < upper{
            return (lower, upper);
        }
        println!("Lower boundry cannot be greater than upper!")
    }
}

fn main(){
    let (lower, upper) = input_range();
    print!("Primes in range [{lower}, {upper}]: (");
    for i in lower..upper+1{
        if is_prime(i){
            if i == lower { print!("{lower}") }
            else{ print!(", {i}") }
        }
    }
    println!(")");
}