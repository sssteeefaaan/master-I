use std::io::{stdin, stdout, Write};

fn gcd(a : u128, b : u128) -> u128{
    if b == 0 {
        return a;
    }
    return gcd(b, a % b);
}

fn left_factorial(n:u8) -> u128{
    if n == 0 {
        panic!("Undefiend!");
    }
    let mut ret = 1_u128;
    for i in 1..n{
        ret += right_factorial(i);
    }
    return ret;
}

fn right_factorial(n:u8) -> u128{
    if n < 2 {
        return 1;
    }
    return n as u128 * right_factorial(n - 1);
}

fn input() -> u8 {
    let mut s = String::new();
    loop{
        print!("n: ");
        let _ = stdout().flush();
        s.clear();
        stdin().read_line(&mut s).expect("Wrong input!");
        match s.trim().parse::<u8>(){
            Ok(value) => return value,
            Err(..) => println!("Wrong input!")
        }
    }
}

fn main(){
    let n = input();
    for i in 2..n+1{
        println!("{i}: gcd(!{i}, {i}!) = {}", gcd(left_factorial(i), right_factorial(i)));
    }
}