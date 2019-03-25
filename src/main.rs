include!(concat!(env!("OUT_DIR"), "/bindings.rs"));

use std::io;
use std::process::Command;


// Bytes in second
unsafe fn call_set_rate(rate :u32, bucket :u32){
    let rate = rate * 1000 / 8;
    set_rate ( rate as i32,
               bucket as i32);
}


fn main() {
    loop {
        println!("Please input the rate(kbits per second): ");
        let mut rate = String::new();
        io::stdin().read_line(&mut rate)
            .expect("Failed to read line.");
        let rate = match rate.trim().as_ref() {
            "" => break,
            rate => {
                rate.parse::<u32>()
                    .expect("Please input a positive integer.")
            }
        };
        println!("Please input the bucket(bytes): ");
        let mut bucket = String::new();
        io::stdin().read_line(&mut bucket)
            .expect("Failed to read line.");
        let bucket = match bucket.trim().as_ref() {
            "" => break,
            bucket => {
                bucket.parse::<u32>()
                    .expect("Please input a positive integer.")
            }
        };
        unsafe {
            call_set_rate(rate, bucket);
        }
        let qdisc_output = Command::new("tc")
            .arg("qdisc")
            .arg("show")
            .output()
            .unwrap();
        let qdisc_output = String::from_utf8_lossy(&qdisc_output.stdout).into_owned();
        let qdisc_output: Vec<&str> = qdisc_output.split("\n").collect();
        let my_qdisc: Vec<&str> = qdisc_output.iter()
            .filter(|output| output.contains("tbf"))
            .cloned()
            .collect();

        println!("The qdisc you set is as followed:");
        for s in my_qdisc {
            println!("{}", s)
        }
    }
}
