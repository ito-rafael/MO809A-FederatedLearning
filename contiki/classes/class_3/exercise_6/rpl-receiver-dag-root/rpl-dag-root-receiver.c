/*
 * ===================
 * rpl-dag-root-receiver.c
 * ===================
 *  class 3: 
 *    code: rpl-dag-root-receiver.c
 *    date: 2022-09-22
 *    solution: rpl-root-receiver_solution.c
 * ===================
 * What to do: 
 *
 * Modify the unicast-example project (exercise_4) from the previous lecture as follows:
 *   - extend the unicast-receiver to behave as DAG ROOT.
 *   - deploy several (e.g., 3) unicast-senders in a line, so that they are neighbors only with the adjacent ones.
 *   - unicast-senders send their messages to the DAG ROOT, using its global address.
 *   - the motes print all their addresses in a loop.
 *   - enable DEBUG_PRINT to see RPL outputs.
 *
 * Tips:
 *   - Sky motes in Cooja have the following link-local unicast addresses:
 *     0xfe80, 0, 0, 0, 0x0212, 0x7401, 0x0001, 0x0101 (where red numbers are the node ID in Cooja)
 *   - Z1 motes in Cooja have the following link-local unicast addresses:
 *     0xfe80, 0, 0, 0, 0xc30c, 0, 0, 0x0001
 */

//============================
// includes
//============================
// general
#include "contiki.h"
#include <stdio.h>
#include "lib/random.h"
//----------------------------
// timer
#include "sys/etimer.h"
//----------------------------
// IPv6
#include "net/ip/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ip/uip-debug.h"
//----------------------------
// UDP
#include "simple-udp.h"
//----------------------------
// RPL
#include "net/rpl/rpl.h"
//----------------------------

//============================
// timer
//============================
static struct etimer timer_rpl;
#define SEND_TIME ((random_rand() % 5) * CLOCK_SECOND)

//============================
// UDP setup
//============================
#define LOCAL_UDP_PORT 1234
#define REMOTE_UDP_PORT 1234
//----------------------------
// create connection
static struct simple_udp_connection conn_dag_root;

//============================
// functions
//============================

//----------------------------
// fn: set global IPv6 address
//----------------------------
static void set_global_addr(){
    // set the 64 MSB of the global IP address
    static uip_ipaddr_t ipaddr_dag_root;
    uip_ip6addr(&ipaddr_dag_root, 0xabcd, 0, 0, 0, 0, 0, 0, 0);
    uip_ds6_set_addr_iid(&ipaddr_dag_root, &uip_lladdr);
    uip_ds6_addr_add(&ipaddr_dag_root, 0, ADDR_AUTOCONF);
    //----------------------------
    // initialize the RPL module as the DAG ROOT node
    rpl_dag_t *dag;
    dag = rpl_set_root(RPL_DEFAULT_INSTANCE,(uip_ip6addr_t *)&ipaddr_dag_root);
    uip_ip6addr(&ipaddr_dag_root, 0xabcd, 0, 0, 0, 0, 0, 0, 0);
    // set the 64 LSB of the global IP address
    rpl_set_prefix(dag, &ipaddr_dag_root, 64);
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
// fn to receive data
//----------------------------
static void receiver(
        struct simple_udp_connection *c,
        const uip_ipaddr_t *sender_addr,
        uint16_t sender_port,
        const uip_ipaddr_t *receiver_addr,
        uint16_t receiver_port,
        const uint8_t *data,
        uint16_t datalen){

   // debug print
   #if CONTIKI_TARGET_Z1
   printf("DAG-ROOT: [Z1 mote] receiving unicast message from ");
   #else
   printf("DAG-ROOT: [Sky mote] receiving unicast message from ");
   #endif

   // print message
   uip_debug_ipaddr_print(sender_addr); 
   printf("\nDAG-ROOT: [message received]: \"%s\"\n", data);
}

//============================
// process setup
//============================
// declare the process
PROCESS(proc_dag_root, "Exercise 6: RPL DAG ROOT receiver");
// start process after Contiki boot
AUTOSTART_PROCESSES(&proc_dag_root);
// define the process
PROCESS_THREAD(proc_dag_root, ev, data) {
    PROCESS_BEGIN();

    //--------------------
    // configuration
    //--------------------
    // set global IPv6 address
    set_global_addr();
    //--------------------
    // set timer and waits it expire
    etimer_set(&timer_rpl, SEND_TIME);
    //--------------------
    // set up the connection
    simple_udp_register(&conn_dag_root, LOCAL_UDP_PORT, NULL, REMOTE_UDP_PORT, receiver);
    //--------------------

    while(1){
        // print addresses
        print_addr();
        // wait timer end
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer_rpl));
    }

    PROCESS_END();
}
