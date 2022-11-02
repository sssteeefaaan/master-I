use std::{io::{stdin, stdout, Write}, str::FromStr};

pub fn input<T>(description : &str) -> T
where T:FromStr {
    let mut s = String::new();
    loop{
        print!("{description}");
        let _ = stdout().flush();
        s.clear();
        stdin().read_line(&mut s).expect("Wrong input!");
        match s.trim().parse::<T>(){
            Ok(value) => return value,
            Err(..) => println!("Wrong input!")
        }
    }
}

pub fn calculator(){
    loop{
        let (a, op, b) = (input::<f64>("a: "), input::<String>("operation: "), input::<f64>("b: "));
        match op.as_str() {
            "+" => break println!("{a} + {b} = {}", a + b),
            "-" => break println!("{a} - {b} = {}", a - b),
            "*" => break println!("{a} * {b} = {}", a * b),
            "/" => break println!("{a} / {b} = {}", a / b),
            "%" => break println!("{a} % {b} = {}", a % b),
            "^" => break println!("{a} ^ {b} = {}", a.powf(b)),
            _ => println!("Wrong operation '{}'!", op)
        }
    }
}

pub fn main(){
    loop{
        let option = input::<u8>("Main menu:\n\t1 - New operation\n\t2 - Exit\n");
        match option {
            1 => calculator(),
            2 => break,
            _ => println!("Wrong option '{option}'!")
        }
    }
}