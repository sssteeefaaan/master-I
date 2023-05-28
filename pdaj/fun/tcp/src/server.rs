use std::{net::TcpListener, io::{BufReader, BufRead}, thread};

pub fn main(address: &String) {
    let s = TcpListener::bind(address).unwrap();
    println!("Successfully connected to {}", address);

    for incoming in s.incoming(){
        if let Ok(stream) = incoming{
            thread::spawn(move||{
                let socket_address = stream.peer_addr().unwrap();
                println!("Established connection to {}", socket_address);

                let mut input;
                while {
                    input = String::new();
                    let mut reader = BufReader::new(&stream);
                    reader.read_line(&mut input).is_ok()
                } {
                    if input.trim().to_lowercase() == "\\quit"{
                        println!("Client {} disconnected!", socket_address);
                        if stream.shutdown(std::net::Shutdown::Both).is_err(){
                            println!("[Error] failed to close connection to {}", socket_address);
                        }
                        return;
                    }
                    println!("{}: {}", socket_address, input.trim());
                }
                println!("[Error] reading from: {}", socket_address);
                if stream.shutdown(std::net::Shutdown::Both).is_err(){
                    println!("[Error] failed to close connection to {}", socket_address);
                }
            });
        }
    }
}