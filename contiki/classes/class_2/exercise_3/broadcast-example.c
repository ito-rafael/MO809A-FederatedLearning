/*
 * ===================
 *  class 2: 
 *  2022-09-20
 *  broadcast-example.c
 * ===================
 * Write a program that:
 *   - in a loop, sets a timer to a random number of seconds that is below X. 
 *   - every time the timer expires, the program senses the temperature and sends it in broadcast. 
 *   - anytime data are received, the program prints out: 
 *     - the sender address and port
 *     - the receiver port, the data, and the data length.
 *   - test it in Cooja using three motes.
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
// motes: 
//   - Zolertia Z1
//   - SkyMote
//============================
//----------------------------
// temperature sensor
//----------------------------
#if CONTIKI_TARGET_Z1
#include "dev/tmp102.h"
#else
#include "dev/sht11/sht11-sensor.h"
#endif

//============================
// UDP setup
//============================
#define LOCAL_UDP_PORT 1234
#define REMOTE_UDP_PORT 1234
//----------------------------
// create connection
static struct simple_udp_connection conn_broadcast;
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
    printf("Sender address: ");
    uip_debug_ipaddr_print(sender_addr);
    printf("; Sender port: %d; Receiver port: %d; Data: %d deg. Celsius; Data length: %d.\n", sender_port, receiver_port, *data, datalen);
}
//----------------------------
// IPv6 address
static uip_ipaddr_t ipaddr;

//============================
// timer
//============================
static struct etimer timer_conn;
#define SEND_TIME ((random_rand() % 5) * CLOCK_SECOND)

//============================
// process setup
//============================
// declare the process
PROCESS(conn_ipv6_udp, "Exercise 3: UDP broadcast");
// start process after Contiki boot
AUTOSTART_PROCESSES(&conn_ipv6_udp);
// define the process
PROCESS_THREAD(conn_ipv6_udp, ev, data) {
    PROCESS_BEGIN();

    //--------------------
    // sensors activation
    //--------------------
    #if CONTIKI_TARGET_Z1
    SENSORS_ACTIVATE(tmp102);
    #else
    SENSORS_ACTIVATE(sht11_sensor);
    #endif

    //--------------------
    // configuration
    //--------------------
    // set up the connection
    simple_udp_register(&conn_broadcast, LOCAL_UDP_PORT, NULL, REMOTE_UDP_PORT, receiver);
    // set the address to the broadcast link-local address
    uip_create_linklocal_allnodes_mcast(&ipaddr);
    // set timer and waits it expire
    etimer_set(&timer_conn, SEND_TIME);

    //--------------------
    // set variables
    //--------------------
    int temp;

    while(1){

        // wait timer end
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer_conn));

        // get temperature
        #if CONTIKI_TARGET_Z1
        temp = tmp102.value(TMP102_READ) / 100;
        #else
        temp = (sht11_sensor.value(SHT11_SENSOR_TEMP)/10 - 396) / 10;
        #endif

        // send broadcast message with temperature data
        simple_udp_sendto(&conn_broadcast, &temp, sizeof(temp), &ipaddr);

        // reset timer
        etimer_reset(&timer_conn);
    }

    PROCESS_END();
}
