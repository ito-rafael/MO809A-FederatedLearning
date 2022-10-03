/*
 * ===================
 *  class 3: 
 *  2022-09-22
 *  get-address.c
 * ===================
 * Write a program that:
 *   - sets a global IPv6 address and retrieves all the IPv6 addresses assigned to the interface of the node.
 */

//============================
// includes
//============================
// general
#include "contiki.h"
#include <stdio.h>
//----------------------------
// IPv6
#include "net/ip/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ip/uip-debug.h"
//----------------------------

//============================
// process setup
//============================
// declare the process
PROCESS(get_ip_addr, "Exercise 5: get IP addresses of interface");
// start process after Contiki boot
AUTOSTART_PROCESSES(&get_ip_addr);

//----------------------------
// fn: set global IPv6 address
//----------------------------
static void set_global_addr(){
    uip_ipaddr_t ipaddr;
    uip_ip6addr(&ipaddr, 0xabcd, 0, 0, 0, 0, 0, 0, 0);
    uip_ds6_set_addr_iid(&ipaddr, &uip_lladdr);
    uip_ds6_addr_add(&ipaddr, 0, ADDR_AUTOCONF);
}
//----------------------------
// fn: get IPv6 addresses
//----------------------------
static void print_addr(){
    int i;
    printf("IPv6 addresses:\n");
    //cycles for the max number of addresses that the interface can have
    for(i = 0; i < UIP_DS6_ADDR_NB; i++){
        if(uip_ds6_if.addr_list[i].isused){
            //print the address
            printf("  ");
            uip_debug_ipaddr_print(&uip_ds6_if.addr_list[i].ipaddr);
            printf("\n");
        }
    }
}
//----------------------------
// define the process
PROCESS_THREAD(get_ip_addr, ev, data) {

    PROCESS_BEGIN();

    set_global_addr();
    print_addr();
    
    PROCESS_END();
}
