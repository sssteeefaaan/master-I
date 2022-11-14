use std::collections::HashMap;

fn left_factorial(n : u8, cache: &mut HashMap<String, HashMap<u8, u64>>) -> u64{
    if n < 1{
        panic!("Left factorial is defined for Natural numbers (n > 0) only!!!!")
    }
    let mut cache_it = false;
    let ret = match cache.get(&String::from("left_factorial")){
        Some(lf_cache)=> 
        {
            match lf_cache.get(&n){
                Some(val)=> *val,
                None=>{
                    cache_it = true;
                    let mut sum = 0_u64;
                    for i in 0..n{
                        sum += right_factorial(i, cache);
                    }
                    sum
                }
            }
        },
        None =>{
            cache.insert(String::from("left_factorial"), HashMap::new());
            left_factorial(n, cache)
        }
    };

    if cache_it {
        match cache.get_mut(&String::from("left_factorial")){
            Some(lf_factorial) => lf_factorial.insert(n, ret),
            None=>None
        };
    }

    return ret;
}

fn right_factorial(n : u8, cache: &mut HashMap<String, HashMap<u8, u64>>) -> u64{
    let mut cache_it = false;

    let ret = match cache.get_mut(&String::from("right_factorial")){
        Some(rf_cache)=>{
            match rf_cache.get(&n){
            Some(val)=> *val,
            None => {
                cache_it = true;
                if n < 2 { 1 } else{ u64::from(n) * right_factorial(n - 1, cache) }
                }
            }
        }
        None => {
            cache.insert(String::from("right_factorial"), HashMap::new());
            right_factorial(n, cache)
        }
    };

    if cache_it {
        match cache.get_mut(&String::from("right_factorial")){
            Some(rf_cache)=>rf_cache.insert(n, ret),
            None=>None
        };
    }
    return ret;
}

fn gcd(a:u64, b:u64) -> u64{
    if b == 0{
        a
    }else{
        gcd(b, a % b)
    }
}

fn main(){
    let n = 21u8;
    let mut cache = HashMap::new();
    for i in 1..n{
        let lf = left_factorial(i, &mut cache);
        let rf = right_factorial(i, &mut cache);
        println!("{i}: gcd(right_factorial({i}), left_factorial({i})) = gcd({rf}, {lf}), {}", gcd(lf, rf));
    }
}