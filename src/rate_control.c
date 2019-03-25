//
// Created by lam on 19-3-23.
//

#include "rate_control.h"
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h> //inet_ntop
#include <netlink/netlink.h>
#include <netlink/genl/genl.h>
#include <netlink/netlink.h>
#include <netlink/utils.h>
#include <netlink/route/rtnl.h>
#include <netlink/route/link.h>
#include <netlink/route/tc.h>
#include <netlink/route/qdisc.h>
#include <netlink/route/qdisc/tbf.h>

/*
 * rate: New rate in bytes per second.
 * bucket: Size of bucket in bytes.
 */
int set_rate(int rate, int bucket) {
    struct nl_sock *sock;
    struct rtnl_qdisc *q;
    struct nl_cache *cache;
    struct rtnl_link *link;
    int if_index;
    int maj = 1;
    int min = 0;

    sock = nl_socket_alloc();
    if (nl_connect(sock, NETLINK_ROUTE) < 0) {
        return -1;
    }

    if (rtnl_link_alloc_cache(sock, AF_UNSPEC, &cache) < 0) {
        return -2;
    }

    link = rtnl_link_get_by_name(cache, "ens33");
    if_index = rtnl_link_get_ifindex(link);

    q = rtnl_qdisc_alloc();

    rtnl_tc_set_ifindex(TC_CAST(q), if_index);
    rtnl_tc_set_parent(TC_CAST(q), TC_H_ROOT);
    rtnl_tc_set_handle(TC_CAST(q), TC_HANDLE(maj, min));
    rtnl_tc_set_kind(TC_CAST(q), "tbf");

    /*
     * netem okay, htb okay, please comment
     * and uncomment the special parameters for the qdiscs
     */
    rtnl_qdisc_tbf_set_limit(q, rate);
    rtnl_qdisc_tbf_set_rate(q, rate, bucket, 0);
    /*
     * rtnl_netem_set_delay(q, 100);
     * rtnl_netem_set_loss(q, 10);
     */

    rtnl_qdisc_add(sock, q, NLM_F_CREATE);

    rtnl_qdisc_put(q);
    nl_socket_free(sock);
    rtnl_link_put(link);
    nl_cache_put(cache);
    return 0;
}