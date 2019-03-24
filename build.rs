extern crate bindgen;

use std::env;
use std::path::PathBuf;

fn main() {
    let lib_path = PathBuf::from(env::current_dir().unwrap().join("build"));

    println!("cargo:rustc-link-search={}", lib_path.display());
    println!("cargo:rustc-link-lib=static=_setrate");
    println!("cargo:rustc-link-lib=nl-genl-3");
    println!("cargo:rustc-link-lib=nfnetlink");
    println!("cargo:rustc-link-lib=nl-route-3");
    println!("cargo:rustc-link-lib=nl-3");

    let bindings = bindgen::Builder::default()
        .header("wrapper.h")
        .whitelist_function("set_rate")
        .generate()
        .expect("unable to generate libnl bindings");

    let out_path = PathBuf::from(env::var("OUT_DIR").unwrap());
    bindings.write_to_file(out_path.join("bindings.rs"))
        .expect("couldn't write bindings!");
}
