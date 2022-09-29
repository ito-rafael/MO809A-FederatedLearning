/*
 * ===================
 * unicast-receiver.c
 * ===================
 *  class 2: 
 *    code: unicast-receiver.c
 *    date: 2022-09-20
 *    solution: unicast-receiver_solution.c
 * ===================
 * Modify the previous code (exercise_3: broadcast_example.c) to produce two separate codes:
 *   - one code only processes received packets. 
 *   - the other code periodically sends unicast messages to the receiver node.
 *   - the message content is a counter that is incremented each time.
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
static struct simple_udp_connection conn_unicast;
//----------------------------
// IPv6 address
static uip_ipaddr_t ipaddr_tx;
//----------------------------
// fn to receive data
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
   printf("Rx: [Z1 mote] receiving unicast message from ");
   #else
   printf("Rx: [Sky mote] receiving unicast message from ");
   #endif

   // print message
   uip_debug_ipaddr_print(&ipaddr_tx); 
   printf("\nRx: [message received]: \"%s\"\n", data);
}

//============================
// process setup
//============================
// declare the process
PROCESS(proc_unicast_receiver, "Exercise 5: UDP unicast receiver");
// start process after Contiki boot
AUTOSTART_PROCESSES(&proc_unicast_receiver);
// define the process
PROCESS_THREAD(proc_unicast_receiver, ev, data) {
    PROCESS_BEGIN();

    //--------------------
    // configuration
    //--------------------
    // set the sender/receiver addresses to a unicast link-local address
    // by convention: 
    //   - if sender == Z1 mote --> receiver = Z1 mote
    //   - if sender == Sky mote --> receiver = Sky mote
    // Z1 mote address: 0xfe80, 0, 0, 0, 0xc30c, 0, 0, 0x000<ID>
    // Sky mote address: 0xfe80, 0, 0, 0, 0x0212, 0x740<ID>, 0x000<ID>, 0x0<ID>0<ID>
    #if CONTIKI_TARGET_Z1  // set sender address as Z1 mote as well with ID=1
    uip_ip6addr(&ipaddr_tx, 0xfe80, 0, 0, 0, 0xc30c, 0, 0, 0x0001);
    #else  // set sender address as Sky mote as well with ID=1
    uip_ip6addr(&ipaddr_tx, 0xfe80, 0, 0, 0, 0x0212, 0x7401, 0x0001, 0x0101);
    #endif

    // set up the connection
    simple_udp_register(&conn_unicast, LOCAL_UDP_PORT, &ipaddr_tx, REMOTE_UDP_PORT, receiver);

    while(1){
        // waits forever
        PROCESS_WAIT_EVENT();
    }

    PROCESS_END();
}
