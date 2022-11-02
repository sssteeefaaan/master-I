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

fn get_vector_from_user() -> Vec<i64> {
    use std::io::{ stdin, stdout, Write };

    let mut v = Vec::new();
    let mut s = String::new();
    let mut trimmed : &str;

    println!("Input vector elements!");

    loop {
        print!("Enter a whole number or break: ");
        let _ = stdout().flush();

        s.clear();
        stdin()
            .read_line(&mut s)
            .expect("Did not enter a correct string");

        trimmed = s.trim();

        if trimmed.eq("break"){
            break; 
        }

        match trimmed.parse::<i64>() {
            Ok(i) => v.push(i),
            Err(..) => println!("Input '{}' was not an integer!", trimmed),
        };
    }

    return v;
}

fn main() {
    let mut v = get_vector_from_user();
    let size = v.len();

    print!("Unsorted vector: ");
    display_vector(&v);

    quick_sort(&mut v, 0, size as i64 - 1);

    print!("Sorted vector: ");
    display_vector(&v);
}