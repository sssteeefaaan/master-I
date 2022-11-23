pub fn read_input(message: &str) -> String{
    use std::io::{stdin, stdout, Write};

    let mut buf = "".to_string();

    loop {

        print!("{message}");
        _ = stdout().flush();

        buf.clear();
        stdin().read_line(&mut buf).expect("Wrong input!");

        buf = buf.trim().to_string();

        if buf.is_empty(){
            println!("Cannot parse empty string!")
        }
        else{
            return buf;
        }
    }
}