use std::{collections::HashMap, result::Result, env};

struct Calculator{
    functions: HashMap::<&'static str, fn(Vec<i64>)->Result<i64, &'static str>>
}

impl Calculator{
    fn new()->Self{
        let mut ret = Self{
            functions:HashMap::<&'static str, fn(Vec<i64>)->Result<i64, &'static str>>::new()
        };

        ret.functions.insert("SUM", |arguments: Vec<i64>| -> Result<i64, &'static str>{
            if arguments.len() < 2{
                return Err("ERROR");
            }
            let mut sum = 0;
            for arg in arguments{
                sum += arg;
            }
            Ok(sum)
        });

        ret.functions.insert("SUB", |arguments: Vec<i64>| -> Result<i64, &'static str>{
            if arguments.len() != 2{
                return Err("ERROR");
            }
            Ok(arguments[0] - arguments[1])
        });

        ret.functions.insert("DIV", |arguments: Vec<i64>| -> Result<i64, &'static str>{
            if arguments.len() != 2 || arguments[1] == 0{
                return Err("ERROR");
            }
            Ok(arguments[0] / arguments[1])
        });

        ret.functions.insert("MUL", |arguments: Vec<i64>| -> Result<i64, &'static str>{
            if arguments.len() < 2{
                return Err("ERROR");
            }
            let mut prod = 1;
            for arg in arguments{
                prod *= arg;
            }
            Ok(prod)
        });

        ret.functions.insert("REM", |arguments: Vec<i64>| -> Result<i64, &'static str>{
            if arguments.len() != 2 || arguments[1] == 0{
                return Err("ERROR");
            }
            Ok(arguments[0] % arguments[1])
        });

        ret.functions.insert("POW", |arguments: Vec<i64>| -> Result<i64, &'static str>{
            if arguments.len() != 2{
                return Err("ERROR");
            }
            Ok(arguments[0].pow(arguments[1] as u32))
        });

        ret
    }

    fn parse_parameters(&self, parameters: &str) -> Result<Vec<i64>, &'static str>{
        let mut ret = Vec::<i64>::new();
        let mut parsed_parameter = String::new();
        let mut level = 0;
        for p in parameters.chars(){
            match p {
                '(' => level += 1,
                ')' => level -= 1,
                ',' => {
                    if level == 0{
                        ret.push(self.evaluate(&parsed_parameter)?);
                        parsed_parameter = String::new();
                        continue;
                    }
                }
                _ => ()
            }
            parsed_parameter.push(p);
        }
        if level != 0 {
            Err("ERROR")
        }else{
            ret.push(self.evaluate(&parsed_parameter)?);
            Ok(ret)
        }
    }

    fn calculate(&self, parameters : &str) -> Result<i64, &'static str>{
        if parameters.len() < 8 || parameters.contains("-"){
            return Err("ERROR")
        }
        let func = &parameters[0..3];
        let func_parameters = &parameters[4..&parameters.len()-1];
        match self.functions.get(func){
            Some(function)=>{
                Ok(function(self.parse_parameters(func_parameters)?)?)
            }
            None => Err("ERROR")
        }
    }

    fn evaluate(&self, parameters: &str) -> Result<i64, &'static str>{
        let ret = parameters.parse::<i64>();
        if ret.is_err(){
            Ok(self.calculate(parameters.replace(" ", "").as_str())?)
        }else{
            Ok(ret.unwrap())
        }
    }
}
fn main() -> Result<(), &'static str> {

    let args:Vec<String> = env::args().collect();

    let calculator = Calculator::new();

    if let Some(expression) = args.get(1){
        println!("{} = {}", expression, calculator.calculate(expression.as_str())?);
        Ok(())
    }else{
        Err("ERROR")
    }
}
