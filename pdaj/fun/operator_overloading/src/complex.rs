use std::{ops::{Add, Sub, Mul, Div}, fmt::Display};


#[derive(Copy, Clone)]
pub struct Complex<T : Copy>{
    pub re: T,
    pub im: T
}

impl<Lhs, Rhs> Add<Complex<Rhs>> for Complex<Lhs>
where Lhs: Add<Rhs, Output = Lhs> + Copy, Rhs : Copy{
    type Output = Complex<Lhs>;

    fn add(self, rhs: Complex<Rhs>) -> Self::Output {
        Self::Output{
            re: self.re + rhs.re,
            im: self.im + rhs.im
        }
    }
}

impl<Lhs, Rhs> Sub<Complex<Rhs>> for Complex<Lhs>
where Lhs: Sub<Rhs, Output = Lhs> + Copy,
Rhs : Copy{
    type Output = Complex<Lhs>;

    fn sub(self, rhs: Complex<Rhs>) -> Self::Output {
        Self::Output{
            re: self.re - rhs.re,
            im: self.im - rhs.im
        }
    }
}

impl<Lhs, Rhs> Mul<Complex<Rhs>> for Complex<Lhs>
where Lhs: Mul<Rhs, Output = Lhs> + Sub<Lhs, Output = Lhs> + Add<Lhs, Output = Lhs> + Copy, Rhs: Copy{
    type Output = Complex<Lhs>;

    fn mul(self, rhs: Complex<Rhs>) -> Self::Output {
        Self::Output{
            re: self.re * rhs.re - self.im * rhs.im,
            im: self.re * rhs.im + self.im * rhs.re
        }
    }
}

impl<Lhs, Rhs> Div<Complex<Rhs>> for Complex<Lhs>
where Lhs: Div<Rhs, Output = Lhs> + Mul<Rhs, Output = Lhs> + Sub<Lhs, Output = Lhs> + Add<Lhs, Output = Lhs> + Copy, Rhs: Copy + Add<Output = Rhs> + Mul<Output = Rhs>{
    type Output = Complex<Lhs>;

    fn div(self, rhs: Complex<Rhs>) -> Self::Output {
        let rhs_mod = rhs.mod_squared();
        Self::Output{
            re: (self.re * rhs.re + self.im * rhs.im) / rhs_mod,
            im: (self.im * rhs.re - self.re * rhs.im) / rhs_mod
        }
    }
}

impl<T : Copy + Display> Display for Complex<T>{
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "{}+{}i", self.re, self.im)
    }
}

impl<T : Copy + Add<Output = T> + Mul<Output = T>> Complex<T>{
    fn mod_squared(&self) -> T {
        self.re * self.re + self.im * self.im
    }
}

pub fn main(){
    let a = Complex{re: 5.3, im: -3.9};
    let b = Complex{re: 0.3, im: 12.9};


    println!("{} + {} = {}", a, b, a + b);
    println!("{} - {} = {}", a, b, a - b);
    println!("({}) * ({}) = {}", a, b, a * b);
    println!("({}) / ({}) = {}", a, b, a / b);
}