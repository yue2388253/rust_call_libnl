# rust_call_libnl

This repository is a simple example of setting rate of the host by using bindgen to call C lib [libnl3](https://www.infradead.org/~tgr/libnl/doc/core.html#core_netlink_fundamentals).

# Prerequisite
```sudo apt install libnl-3-dev libnl-route-3-dev libnl-route-3-dev libnfnetlink-dev```


# How to use this repo

**Simply run ```make run``` is enough.** This command would compile the c code and generate a static lib (*lib_setrate.a*) in directory *build*, and then run ```cargo run``` automatically.

```make``` : only compile the static lib (*lib_setrate.a*)

```make clean``` : clean all the stuff that the commands above generate.