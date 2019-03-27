//
// Created by lam on 19-3-23.
//
#include "rate_control.h"
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h> //inet_ntop
#include <netlink/netlink.h>
#include <netlink/utils.h>
#include <netlink/genl/genl.h>
#include <netlink/route/rtnl.h>
#include <netlink/route/link.h>
#include <netlink/route/tc.h>
#include <netlink/route/qdisc.h>
#include <netlink/route/qdisc/tbf.h>
/*
 * rate: New rate in bytes per second.
 * bucket: Size of bucket in bytes.
 */
int add_qdisc_tbf(char const* if_name, int maj, int min, int limit, int rate, int bucket)
{
    struct nl_sock *sock;
    struct rtnl_qdisc *q;
    struct nl_cache *cache;
    struct rtnl_link *link;
    int if_index;
    sock = nl_socket_alloc();
    if (sock == NULL)
        return -1;
    if (nl_connect(sock, NETLINK_ROUTE) < 0)
        return -2;
    if (rtnl_link_alloc_cache(sock, AF_UNSPEC, &cache) < 0)
        return -3;
    link = rtnl_link_get_by_name(cache, if_name);
    nl_cache_free(cache);
    if (link == NULL)
        return -4;
    if_index = rtnl_link_get_ifindex(link);
    if (if_index == 0)
        return -5;
    q = rtnl_qdisc_alloc();
    if (q == NULL)
        return -6;
    rtnl_tc_set_ifindex(TC_CAST(q), if_index);
    rtnl_tc_set_parent(TC_CAST(q), TC_H_ROOT);
    rtnl_tc_set_handle(TC_CAST(q), TC_HANDLE(maj,min));
    if (rtnl_tc_set_kind(TC_CAST(q), "tbf") < 0)
        return -7;

    //https://linux.die.net/man/8/tc-tbf
    rtnl_qdisc_tbf_set_limit(q, limit);
    rtnl_qdisc_tbf_set_rate(q, rate, bucket, 0);
    /*
    netem okay, htb okay, please comment
    and uncomment the special parameters for the qdiscs
    rtnl_netem_set_delay(q, 100);
    rtnl_netem_set_loss(q, 10);
    */

    if (rtnl_qdisc_add(sock, q, NLM_F_CREATE) < 0)
        return -10;
    rtnl_qdisc_put(q);
    rtnl_link_put(link);
    
    nl_socket_free(sock);
    return 0;
}


int delete_qdisc(char const * if_name, int maj, int min){
    struct nl_sock *sock;
    struct rtnl_qdisc *q;
    struct nl_cache *cache;
    struct rtnl_link *link;
    int if_index;
    sock = nl_socket_alloc();
    
    if (sock == NULL)
        return -1;
    if (nl_connect(sock, NETLINK_ROUTE) < 0)
        return -2;
    if (rtnl_link_alloc_cache(sock, AF_UNSPEC, &cache) < 0)
        return -3;
    link = rtnl_link_get_by_name(cache, if_name);
    if (link == NULL)
        return -4;
    nl_cache_free(cache);
    if_index = rtnl_link_get_ifindex(link);
    if (if_index == 0)
        return -5;
    q = rtnl_qdisc_alloc();
    if (q == NULL)
        return -6;

    rtnl_tc_set_ifindex(TC_CAST(q), if_index);
    rtnl_tc_set_parent(TC_CAST(q), TC_H_ROOT);
    rtnl_tc_set_handle(TC_CAST(q), TC_HANDLE(maj,min));

    if (rtnl_qdisc_delete(sock, q) < 0)
        return -10;
    rtnl_qdisc_put(q);
    rtnl_link_put(link);
    nl_socket_free(sock);
    return 0;
}