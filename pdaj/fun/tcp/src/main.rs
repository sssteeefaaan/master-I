use std::{env};

mod client;
mod server;

fn main() -> Result<(), String>{
    let args : Vec<String> = env::args().collect();
    if args.len() < 3{
        Err("Two arguments are required, client/server and address".to_string())
    }else{
        match args[1].to_lowercase().as_str(){
            "client" =>{
                client::main(&args[2]);
                Ok(())
            }
            "server"=>{
                server::main(&args[2]);
                Ok(())
            },
            _=>{
                Err("client/server are only available options!".to_string())
            }
        }
    }
}
