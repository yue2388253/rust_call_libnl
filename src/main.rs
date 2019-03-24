include!(concat!(env!("OUT_DIR"), "/bindings.rs"));

use std::io;


// Bytes in second
unsafe fn call_set_rate(rate :u32, bucket :u32){
    let rate = rate * 1000 / 8;
    set_rate ( rate as i32,
               bucket as i32);
}


fn main() {
    println!("Please input the rate(kbits per second): ");
    let mut rate = String::new();
    io::stdin().read_line(&mut rate)
        .expect("Failed to read line.");
    let rate = rate.trim()
        .parse::<u32>()
        .expect("Please input a positive integer.");
    println!("Please input the bucket(bytes): ");
    let mut bucket = String::new();
    io::stdin().read_line(&mut bucket)
        .expect("Failed to read line.");
    let bucket = bucket.trim()
        .parse::<u32>()
        .expect("Please input a positive integer.");
    unsafe {
        call_set_rate(rate, bucket);
    }
}
