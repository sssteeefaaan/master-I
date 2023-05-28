use std::{net::TcpStream, io::{Write, stdin}};

pub fn main(address: &String) {
    let mut c = TcpStream::connect(address).unwrap();
    println!("Successfully connected to {}", address);

    let mut input;

    loop{
        println!("Enter text or \\quit to exit: ");

        input = String::new();
        stdin().read_line(&mut input).expect("Couldn't read the input!");

        if c.write_all(format!("{}", input).as_bytes()).is_err() || c.flush().is_err(){
            println!("Error sending data!");
        }

        if input.trim().to_lowercase() == "\\quit"{
            break;
        }
    }
    c.shutdown(std::net::Shutdown::Both).expect("Error closing the socket!");
}