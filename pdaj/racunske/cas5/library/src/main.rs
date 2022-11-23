pub mod library;
pub mod utilities;

fn menu(){
    use std::collections::HashMap;

    let mut db = HashMap::new();
    loop{
        let mut message = ["="; 20].join("=");
        message.push_str("\nMain menu:\n");
        message.push_str("\t1 - Create a new library\n");
        message.push_str("\t2 - Show all libraries\n");
        message.push_str("\t3 - Enter a library\n");
        message.push_str("\t4 - Exit\n");
        message.push_str("Select option(1-4): ");
        match utilities::read_input(message.as_str()).as_str() {
            "1" => library::create_menu(&mut db),
            "2" => library::display_all_libraries(&db),
            "3" => library::display_library(&db),
            "4" => break,
            _ => {
                println!("Wrong input! Please try again!")
            }
        }
    }
}
fn main(){
    menu();
}