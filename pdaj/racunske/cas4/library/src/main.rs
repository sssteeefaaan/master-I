use std::collections::HashMap;

#[derive(Debug)]
enum BookCategory{
    ROMANCE,
    DRAMA,
    HORROR,
    ACTION,
    CRIME,
    COMEDY,
    UNKNOWN,
}

impl BookCategory{
    fn from(b:&BookCategory) -> BookCategory{
        match b {
            BookCategory::ROMANCE => BookCategory::ROMANCE,
            BookCategory::DRAMA => BookCategory::DRAMA ,
            BookCategory::HORROR => BookCategory::HORROR,
            BookCategory::ACTION => BookCategory::ACTION,
            BookCategory::CRIME => BookCategory::CRIME,
            BookCategory::COMEDY => BookCategory::COMEDY,
            BookCategory::UNKNOWN => BookCategory::UNKNOWN,
        }
    }
}
#[derive(Debug)]
struct Library{
    name: String,
    books: HashMap<String, Book>,
    users: HashMap<String, User>
}
#[derive(Debug)]
struct Book{
    author: String,
    title: String,
    category: BookCategory,
    isbn: String,
    amount: u16
}

impl Book{
    fn from(b:&Book) -> Book{
        Book{
            author:b.author.clone(),
            title:b.title.clone(),
            category: BookCategory::from(&b.category),
            isbn:b.isbn.clone(),
            amount: b.amount
        }
    }

    fn new() -> Book{
        Book {
            author: String::new(),
            title: String::new(),
            category: BookCategory::UNKNOWN,
            isbn: String::new(),
            amount: 0
        }
    }

    fn create(author:String, title:String, category:BookCategory, isbn:String, amount: u16) -> Book{
        Book{
            author,
            title,
            category,
            isbn,
            amount
        }
    }
}

#[derive(Debug)]
struct User{
    full_name: String,
    id: String,
    borrowed: HashMap<String, Book>
}

impl User{
    fn new() -> User{
        User { full_name:String::new(), id: String::new(), borrowed: HashMap::new() }
    }

    fn create(full_name:String, id:String, borrowed:HashMap<String, Book>) -> User{
        User { full_name, id, borrowed }
    }
}

impl Library{
    fn add_user(&mut self, u:User) -> bool{
        match self.users.get(&u.id){
            Some(_)=>{
                println!("User with '{}' already exists!", u.id);
                false
            },
            None=>{
                self.users.insert(u.id.clone(), u);
                true
            }
        }
    }

    fn add_book(&mut self, b:Book) -> bool{
        match self.books.get(&b.isbn){
            Some(_)=> {
                println!("Book with '{}' already exists!", b.isbn);
                false
            },
            None=>{
                self.books.insert(b.isbn.clone(), b);
                true
            }
        }
    }

    fn borrow(&mut self, user_id:&String, book_isbn:&String, amount:u16) -> bool{
        match self.books.get_mut(book_isbn){
            Some(b) =>{
                match self.users.get_mut(user_id){
                    Some(u)=>{
                        if b.amount >= amount{
                            let mut bc = Book::from(b);
                            bc.amount = amount;
                            u.borrowed.insert(user_id.clone(), bc);
                            b.amount-=amount;
                            
                        }else{
                            println!("Book '{}' only has {} copies left!", b.isbn, b.amount);
                        }
                        true
                    }
                    None => {
                        println!("User '{}' doesn't exist!", user_id);
                        false
                    }
                }
            }
            None => {
                println!("Book '{}' doesn't exist!", book_isbn);
                false
            }
        }
    }
}

fn main(){

    let mut l = Library{
        name: String::from("Town Library"),
        users: HashMap::new(),
        books: HashMap::new(),
    };

    l.add_user(User::create(
        String::from("Petar Petrovic"),
        String::from("0"),
        HashMap::new()

    ));
    l.add_user(User::create(
        String::from("Nikola NIkolic"),
        String::from("1"),
        HashMap::new()

    ));
    l.add_user(User::create(
        String::from("Filip Filipovic"),
        String::from("2"),
        HashMap::new()

    ));

    l.add_book(Book::create(
        String::from("William Shakespeare"),
        String::from("Romeo and Juliet"),
        BookCategory::ROMANCE,
        String::from("2c03ff0c-6403-11ed-ab9f-66a188459d2a"),
        2
    ));
    l.add_book(Book::create(
        String::from("Dante Alighieri"),
        String::from("Inferno"),
        BookCategory::DRAMA,
        String::from("2c040042-6403-11ed-aba0-66a188459d2a"),
        2
    ));
    l.add_book(Book::create(
        String::from("Stephen King"),
        String::from("The Shinning"),
        BookCategory::HORROR,
        String::from("2c040074-6403-11ed-aba1-66a188459d2a"),
        2
    ));

    println!("{:#?}", l);
    println!();
    println!();

    let user_id = String::from("0");
    let book_isbn = String::from("2c040074-6403-11ed-aba1-66a188459d2a");
    l.borrow(&user_id, &book_isbn, 5);
    l.borrow(&user_id, &book_isbn, 1);

    println!("{:#?}", l);
    println!();
    println!();

}