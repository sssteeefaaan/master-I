fn swap(array : &mut Vec<i64>, i : i64, j : i64){
    let temp = array[i as usize];
    array[i as usize] = array[j as usize];
    array[j as usize] = temp;
}

fn partition(array : &mut Vec<i64>, low : i64, high : i64) -> i64{
    let pivot = array[high as usize];
    let mut i = low - 1;
    let mut j = low;
    while j < high {
        if array[j as usize] < pivot {
            i+=1;
            swap(array, i, j);
        }
        j+=1;
    }
    i+= 1;
    swap(array, i, high);
    return i;
}

fn quick_sort(array : &mut Vec<i64>, low : i64, high : i64){
    if low < high{
        let part = partition(array, low, high);
        quick_sort(array, part + 1, high);
        quick_sort(array, low, part - 1);
    }
}

fn display_vector(vec : &Vec<i64>){
    print!("[\t");
    for i in vec{
        print!("{i}\t");
    }
    println!("]");
}

fn get_vector_from_user(v : &mut Vec<i64>){
    use std::io::{stdin,stdout,Write};
    loop {
        let mut s = String::new();
        print!("Enter a whole number or break: ");
        let _ = stdout().flush();
        stdin()
            .read_line(&mut s)
            .expect("Did not enter a correct string");
        let trimmed = s.trim();
        if trimmed.eq("break"){
            break; 
        }
        match trimmed.parse::<i64>() {
            Ok(i) => v.push(i),
            Err(..) => println!("This was not an integer: {}", trimmed),
        };
    }
}

fn main() {
    let mut v = Vec::new();

    get_vector_from_user(&mut v);
    
    let size = v.len() as i64;

    display_vector(&v);

    quick_sort(&mut v, 0, size - 1);

    display_vector(&v);
}
