struct Film{
    naziv: String,
    zanr: String,
    reditelj: String,
    godina: i32,
    ocene: Vec<i32>,
}

struct Serija{
    naziv: String,
    zanr: String,
    ocene: Vec<i32>,
    sezone: Vec<String>
}

trait ImaOcene<T>{
    fn moje_ocene(&mut self) -> &mut Vec<T>;
}

trait ImaZanr{
    fn moj_zanr(&self)-> String;
}

trait ImaNaziv{
    fn moj_naziv(&self)-> String;
}

trait Zabava : ImaOcene<i32> + ImaZanr + ImaNaziv{
    fn naziv(&self) -> String {self.moj_naziv()}
    fn zanr(&self) -> String {self.moj_zanr()}
    fn oceni(&mut self, ocena:i32){
        self.moje_ocene().push(ocena);
    }
    fn prosecna_ocena(&mut self)->f32{
        let n = self.moje_ocene().len();
        if n < 1{
            return 0.0;
        }
        let mut ret = 0;
        for o in self.moje_ocene(){
            ret += o.clone();
        }
        return ret as f32 / n as f32;
    }

    fn prikaz(&mut self){
        println!("{} - {} [{}]", self.naziv(), self.zanr(), self.prosecna_ocena());
    }
}

impl ImaNaziv for Serija{
    fn moj_naziv(&self)-> String {
        self.naziv.clone()
    }
}

impl ImaNaziv for Film{
    fn moj_naziv(&self)-> String {
        self.naziv.clone()
    }
}

impl ImaOcene<i32> for Serija{
    fn moje_ocene(&mut self)-> &mut Vec<i32> {
        &mut self.ocene
    }
}

impl ImaOcene<i32> for Film{
    fn moje_ocene(&mut self)-> &mut Vec<i32> {
        &mut self.ocene
    }
}

impl ImaZanr for Serija{
    fn moj_zanr(&self)-> String {
        self.zanr.clone()
    }
}

impl ImaZanr for Film{
    fn moj_zanr(&self)-> String {
        self.zanr.clone()
    }
}

impl Zabava for Film{
}

impl Zabava for Serija{
}

fn main() {
    let mut f = Film{
        naziv: String::from("123"),
        zanr: String::from("321"),
        reditelj: String::from("567"),
        godina: 2019,
        ocene: Vec::new(),
    };

    let mut s = Serija{
        naziv: String::from("987"),
        zanr: String::from("678"),
        ocene: Vec::new(),
        sezone: Vec::new(),
    };

    f.oceni(5);
    s.oceni(3);

    f.prikaz();
    s.prikaz();
}
