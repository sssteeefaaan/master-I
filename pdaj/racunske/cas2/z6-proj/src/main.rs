use std::collections::HashMap;


#[derive(Debug)]
enum Value{
    INT_HASH(HashMap<u128,u128>),
    GCD_HASH(HashMap<u128, HashMap<u128, u128>>)
}

fn left_factorial(n : u128, cache: &mut HashMap<String, Value>) -> u128{
    if n < 1{
        panic!("Left factorial is defined for Natural numbers (n > 0) only!!!!")
    }
    let mut cache_it = false;
    let ret = match cache.get(&String::from("left_factorial")){
        Some(lf_cache)=> 
        {
            match lf_cache{
                Value::INT_HASH(lf_cache)=>{
                    match lf_cache.get(&n){
                        Some(val)=> *val,
                        None=>{
                            cache_it = true;
                            let mut sum = 0_u128;
                            for i in 0..n{
                                sum += right_factorial(i, cache);
                            }
                            sum
                        }
                    }
                },
                _ => 0
            }
            
        },
        None =>{
            cache.insert(String::from("left_factorial"), Value::INT_HASH(HashMap::new()));
            left_factorial(n, cache)
        }
    };

    if cache_it {
        match cache.get_mut(&String::from("left_factorial")){
            Some(lf_factorial) => {
                match lf_factorial{
                    Value::INT_HASH(lf_cache)=>{
                        lf_cache.insert(n, ret);
                    },
                    _ => ()
                }
            },
            None => ()
        };
    }

    return ret;
}

fn right_factorial(n : u128, cache: &mut HashMap<String, Value>) -> u128{
    let mut cache_it = false;

    let ret = match cache.get_mut(&String::from("right_factorial")){
        Some(rf_cache)=>{
            match rf_cache {
                Value::INT_HASH(rf_cache)=>{
                    match rf_cache.get(&n){
                        Some(val)=> *val,
                        None => {
                            cache_it = true;
                            if n < 2 { 1 } else{ n * right_factorial(n - 1, cache) }
                            }
                        }
                },
                _ => 0
            }
            
        }
        None => {
            cache.insert(String::from("right_factorial"), Value::INT_HASH(HashMap::new()));
            right_factorial(n, cache)
        }
    };

    if cache_it {
        match cache.get_mut(&String::from("right_factorial")){
            Some(rf_cache) => {
                match rf_cache {
                    Value::INT_HASH(rf_cache)=> {rf_cache.insert(n, ret);},
                    _ => ()
                }
            },
            None => ()
        };
    }
    return ret;
}

fn gcd(a:u128, b:u128, cache:&mut HashMap<String, Value>) -> u128{
    let mut cache_it = false;
    let ret = match cache.get_mut(&("gcd".to_string())){
        Some(gcd_cache)=>{
            match gcd_cache{
                Value::GCD_HASH(gcd_cache)=>{
                    match gcd_cache.get_mut(&a){
                        Some(gcd_cache)=>{
                            match gcd_cache.get_mut(&b){
                                Some(val)=> *val,
                                None=>{
                                    let ret =
                                    if b == 0{
                                        a
                                    }else{
                                        cache_it = true;
                                        gcd(b, a % b, cache)
                                    };
                                    ret
                                }
                            }
                
                        },
                        None => {
                            gcd_cache.insert(a, HashMap::new());
                            gcd(a, b, cache)
                        }
                    }
                },
                _ => 0
            }
        },
        None => {
            cache.insert("gcd".to_string(), Value::GCD_HASH(HashMap::new()));
            gcd(a,b,cache)
        }
    };

    if cache_it{
        match cache.get_mut(&"gcd".to_string()){
            Some(gcd_cache) => {
                match gcd_cache {
                    Value::GCD_HASH(gcd_cache)=>{
                        match gcd_cache.get_mut(&a){
                            Some(gcd_cache)=>{gcd_cache.insert(b, ret);},
                            None=>()
                        }
                    },
                    _ => ()
                }
            },
            None => ()
        }
    }

    return ret;
}

fn main(){
    let n = 21u128;
    let mut cache = HashMap::new();
    for i in 1..n{
        let lf = left_factorial(i, &mut cache);
        let rf = right_factorial(i, &mut cache);
        println!("{i}: gcd(right_factorial({i}), left_factorial({i})) = gcd({rf}, {lf}) = {}", gcd(lf, rf, &mut cache));
    }

    println!("{:#?}", cache);
}