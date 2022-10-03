/*
 * ===================
 *  class 4: 
 *  2022-09-27
 *  unicast-server.c
 * ===================
 * Write a client-server application that:
 *
 * Client: 
 *   - must be written in Java using the following code as reference: 
 *     https://systembash.com/a-simple-java-udp-server-and-udp-client/
 *   - in a loop, asks to the user for the message to send to the server.
 *   - if the message is "bye", the client terminates.
 *   - prints out any response from the server.
 *   - run on your host.
 *
 * Server: 
 *   - deploy a network in Cooja with two motes.
 *   - the first mote must be the border router.
 *   - the second mote must be an UDP server that waits for a request from the client and echoes the received data back to the client.
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
#define SERVER_UDP_PORT 1234
#define CLIENT_UDP_PORT 4321
//----------------------------
// create connection
static struct simple_udp_connection conn;

//============================
// UDP server (fn to receive data)
//============================
static void receiver(
        struct simple_udp_connection *c,
        const uip_ipaddr_t *sender_addr,
        uint16_t sender_port,
        const uip_ipaddr_t *receiver_addr,
        uint16_t receiver_port,
        const uint8_t *data,
        uint16_t datalen){

    // print message
    printf("Message received from: ");
    uip_debug_ipaddr_print(sender_addr);
    printf(":%d to ", sender_port);
    uip_debug_ipaddr_print(receiver_addr); 
    printf(":%d; Data length: %d\n", receiver_port, datalen);
    // send echo response
    printf("Replying echo back...\n");
    simple_udp_sendto(c, data, datalen, sender_addr);
}

//============================
// process setup
//============================
// declare the process
PROCESS(udp_server, "Exercise 8: UDP server");
// start process after Contiki boot
AUTOSTART_PROCESSES(&udp_server);
// define the process
PROCESS_THREAD(udp_server, ev, data) {

    PROCESS_BEGIN();

    // set connection
    simple_udp_register(&conn, SERVER_UDP_PORT, NULL, CLIENT_UDP_PORT, receiver);

    while(1){
        // waits forever
        PROCESS_WAIT_EVENT();
    }

    PROCESS_END();
}
