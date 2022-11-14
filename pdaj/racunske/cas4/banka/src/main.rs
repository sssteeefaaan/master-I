#[derive(Debug)]
struct User{
    name:String,
    account: String,
    active: bool,
    age: i8
}
#[derive(Debug)]
#[derive(PartialEq)]
enum AccountType{
    DIN(String),
    DEVIZNI(String)
}

struct Account{
    num:String,
    account_type: AccountType
}

impl User{

    fn new(name:String, account:String, active:bool, age:i8) -> Self{
        Self { name, account, active, age }
    }

    fn from(original: &User) -> Self{
        Self { name:original.name.clone(), account:original.account.clone(), active:original.active, age:original.age }
    }

    fn print(&self){
        println!("{:#?}", self);
    }
}


fn main(){
    let mut pera = User::new(
        String::from("Pera"),
        String::from("2c43b682-63e6-11ed-ad5b-721e3a5ecd25"),
        true,
        22
    );

    let mut mika = User::from(&pera);
    mika.name=String::from("Mika");

    println!("{}", pera.name);

    pera.name = String::from("Perica");
    println!("{}", pera.name);

    pera.print();
    mika.print();

    dbg!(&pera);

    // enum
    let din_acc = AccountType::DIN(String::from("RSD"));
    let dev_acc = AccountType::DEVIZNI(String::from("RSD"));

    dbg!(&din_acc);
    dbg!(&dev_acc);

    dbg!(din_acc == dev_acc);

}