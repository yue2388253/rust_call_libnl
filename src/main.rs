include!(concat!(env!("OUT_DIR"), "/bindings.rs"));

use std::io;
use std::process::Command;
use std::ffi::CString;
use std::os::raw::c_char;


// Bytes in second
unsafe fn call_add_qdisc_tbf(
    if_name : *const c_char,
    maj: u32, min: u32,
    limit: u32, rate: u32,
    bucket: u32
) -> i32 {
    let rate = rate / 8 * 1000;
    let bucket = bucket;
    add_qdisc_tbf(if_name, maj as i32, min as i32,
                  limit as i32, rate as i32, bucket as i32)
}

unsafe fn call_delete_qdisc(
    if_name: *const c_char,
    maj :u32, min :u32
) -> i32 {
    delete_qdisc(if_name, maj as i32, min as i32)
}

fn main() {
	let if_name=CString::new("ens33").unwrap();
    let if_name_ptr=if_name.as_ptr();
    loop {
        println!("Please input the rate(kbits per second): ");
        let mut rate = String::new();
        io::stdin().read_line(&mut rate)
            .expect("Failed to read line.");
        let rate = match rate.trim().as_ref() {
            "" => {
                unsafe {
                    println!("->info delete_qdisc:err->{}", call_delete_qdisc(if_name_ptr, 1, 0));
                    break;
                }
            }

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
        let mut queue_limit = String::new();
        println!("Please input the queue_limit(bytes): ");
        io::stdin().read_line(&mut queue_limit)
            .expect("Failed to read line.");
        let queue_limit = match queue_limit.trim().as_ref() {
            "" => break,
            queue_limit => {
                queue_limit.parse::<u32>()
                    .expect("Please input a positive integer.")
            }
        };

        //https://linux.die.net/man/8/tc-tbf
        unsafe {
        	println!("->info add_qdisc_tbf:err->{}",call_add_qdisc_tbf(if_name_ptr,1,0,queue_limit,rate, bucket));
    	};

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
