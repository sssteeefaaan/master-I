use std::{ops::Add, borrow::Borrow};

pub mod dllist;

#[derive(PartialEq, Eq)]
struct Point{
    pub x : i32,
    pub y : i32
}

impl Point{
    fn distance(&self, p:&Point) -> f64{
        (((self.x - p.x).pow(2).add((self.y - p.y).pow(2))) as f64).sqrt()
    }
}

fn pattern_matching(a : i32){
    match a{
        val @ 0..=55 => println!("Hello {}!", val),
        val @ 56..=99 => println!("Hey {}!", val),
        _ => println!("Hi!")
    }
}

fn pattern_matchings<P: Borrow<Point>>(p : P){
    match p.borrow() {
        Point{x: 5, y: 10} => println!("Bullseye!"),
        Point{x, y : 10} => println!("Go {}", if x < &5 {"right"} else { "left"}),
        Point{x: 5, y } => println!("Go {}", if y < &10 {"up"} else { "down"}),
        Point{x,y} => println!("Missed ({}, {}) by {}!", x, y, Point{x:5,y:10}.distance(p.borrow()))
    }
}
fn main() {
    pattern_matchings(Point{x:10, y:1});
    pattern_matchings(Point{x:1, y:5});
    pattern_matchings(Point{x:5, y:11});
    pattern_matchings(Point{x:5, y:1});
    pattern_matchings(Point{x:5, y:10});

    dllist::main();
}
