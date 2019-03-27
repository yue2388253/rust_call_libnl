//
// Created by lam on 19-3-23.
//

#ifndef RUST_CALL_LIBNL_RATE_CONTROL_H
#define RUST_CALL_LIBNL_RATE_CONTROL_H

int add_qdisc_tbf(char const* if_name, int maj, int min, int limit, int rate, int bucket);
int delete_qdisc(char const* if_name, int maj, int min);

#endif //RUST_CALL_LIBNL_RATE_CONTROL_H
