# rust_call_libnl

This repository is a simple example of setting rate of the host by using bindgen to call C lib [libnl3](https://www.infradead.org/~tgr/libnl/doc/core.html#core_netlink_fundamentals).

# Prerequisite
```sudo apt install libnl-3-dev libnl-route-3-dev libnl-route-3-dev libnfnetlink-dev```


# How to use this repo

1. ```make``` : compile the static lib (*lib_setrate.a*) and run ```cargo build```. This command will generate a executable file named ***rust_call_libnl*** in directory(***target/build***).  
2. ```sudo ./target/build/rust_call_libnl```: require the user to set a rate and a bucket size, and then set it.
3. ```tc qdisc show```: you will see the qdisc you set just now.

```make clean``` : clean all the stuff that the commands above generate.
