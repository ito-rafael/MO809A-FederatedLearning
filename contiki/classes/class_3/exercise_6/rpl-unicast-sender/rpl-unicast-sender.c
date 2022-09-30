/*
 * ===================
 * rpl-unicast-sender.c
 * ===================
 *  class 3: 
 *    code: unicast-sender.c
 *    date: 2022-09-22
 *    solution: rpl-unicast-sender_solution.c
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
#include <string.h>
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

//============================
// UDP setup
//============================
#define LOCAL_UDP_PORT 1234
#define REMOTE_UDP_PORT 1234
//----------------------------
// create connection
static struct simple_udp_connection conn_rpl;
//----------------------------
// IPv6 address
static uip_ipaddr_t ipaddr_dag_root;

//============================
// timer
//============================
static struct etimer timer_conn;
#define SEND_TIME ((random_rand() % 5) * CLOCK_SECOND)

//============================
// process setup
//============================
// declare the process
PROCESS(proc_rpl_sender, "Exercise 6: RPL unicast sender");
// start process after Contiki boot
AUTOSTART_PROCESSES(&proc_rpl_sender);

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
PROCESS_THREAD(proc_rpl_sender, ev, data) {
    PROCESS_BEGIN();

    //--------------------
    // configuration
    //--------------------
    
    // set the DAG ROOT global address
    // by convention: 
    //   - if sender == Z1 mote --> DAG ROOT == Z1 mote
    //   - if sender == Sky mote --> DAG ROOT == Sky mote
    // Z1 mote address: 0xabcd, 0, 0, 0, 0xc30c, 0, 0, 0x000<ID>
    // Sky mote address: 0xabcd, 0, 0, 0, 0x0212, 0x740<ID>, 0x000<ID>, 0x0<ID>0<ID>
    #if CONTIKI_TARGET_Z1  // set DAG ROOT address as Z1 mote as well with ID=1
    uip_ip6addr(&ipaddr_dag_root, 0xabcd, 0, 0, 0, 0xc30c, 0, 0, 0x0001);
    #else  // set DAG ROOT address as Sky mote as well with ID=1
    uip_ip6addr(&ipaddr_dag_root, 0xabcd, 0, 0, 0, 0x0212, 0x7401, 0x0001, 0x0101);
    #endif

    // set up the connection
    simple_udp_register(&conn_rpl, LOCAL_UDP_PORT, &ipaddr_dag_root, REMOTE_UDP_PORT, NULL);
    // set timer and waits it expire
    etimer_set(&timer_conn, SEND_TIME);

    //--------------------
    // set variables
    //--------------------
    static unsigned int counter = 0;
    static char message[300];

    while(1){

        // wait timer end
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer_conn));
        
        // print IPv6 addresses
        print_addr();

        // debug print
        #if CONTIKI_TARGET_Z1
        printf("Tx: [Z1 mote] sending unicast message to ");
        #else
        printf("Tx: [Sky mote] sending unicast message to ");
        #endif

        // send unicast message with counter
        uip_debug_ipaddr_print(&ipaddr_dag_root);
        sprintf(message, "Counter = %d", counter);
        simple_udp_send(&conn_rpl, message, strlen(message));
        printf("\nTx: [message sent] \"Counter = %d\"\n", counter);
        counter++;

        // reset timer
        etimer_reset(&timer_conn);
    }

    PROCESS_END();
}
