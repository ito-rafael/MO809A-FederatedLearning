/*
 * ===================
 * unicast-sender.c
 * ===================
 *  class 2: 
 *    code: unicast-sender.c
 *    date: 2022-09-20
 *    solution: unicast-sender_solution.c
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
static struct simple_udp_connection conn_unicast;
//----------------------------
// IPv6 address
static uip_ipaddr_t ipaddr_rx;

//============================
// timer
//============================
static struct etimer timer_conn;
#define SEND_TIME ((random_rand() % 5) * CLOCK_SECOND)

//============================
// process setup
//============================
// declare the process
PROCESS(proc_unicast_sender, "Exercise 4: UDP unicast sender");
// start process after Contiki boot
AUTOSTART_PROCESSES(&proc_unicast_sender);
// define the process
PROCESS_THREAD(proc_unicast_sender, ev, data) {
    PROCESS_BEGIN();

    //--------------------
    // configuration
    //--------------------
    
    // set the sender/receiver addresses to a unicast link-local address
    // by convention: 
    //   - if sender == Z1 mote --> receiver == Z1 mote
    //   - if sender == Sky mote --> receiver == Sky mote
    // Z1 mote address: 0xfe80, 0, 0, 0, 0xc30c, 0, 0, 0x000<ID>
    // Sky mote address: 0xfe80, 0, 0, 0, 0x0212, 0x740<ID>, 0x000<ID>, 0x0<ID>0<ID>
    #if CONTIKI_TARGET_Z1  // set receiver address as Z1 mote as well with ID=2
    uip_ip6addr(&ipaddr_rx, 0xfe80, 0, 0, 0, 0xc30c, 0, 0, 0x0002);
    #else  // set receiver address as Sky mote as well with ID=2
    uip_ip6addr(&ipaddr_rx, 0xfe80, 0, 0, 0, 0x0212, 0x7402, 0x0002, 0x0202);
    #endif

    // set up the connection
    simple_udp_register(&conn_unicast, LOCAL_UDP_PORT, &ipaddr_rx, REMOTE_UDP_PORT, NULL);
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
        
        // debug print
        #if CONTIKI_TARGET_Z1
        printf("Tx: [Z1 mote] sending unicast message to ");
        #else
        printf("Tx: [Sky mote] sending unicast message to ");
        #endif

        // send unicast message with counter
        uip_debug_ipaddr_print(&ipaddr_rx);
        sprintf(message, "Counter = %d", counter);
        simple_udp_send(&conn_unicast, message, strlen(message));
        printf("\nTx: [message sent] \"Counter = %d\"\n", counter);
        counter++;

        // reset timer
        etimer_reset(&timer_conn);
    }

    PROCESS_END();
}
