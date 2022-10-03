/*
 * coap://[abcd::c30c:0:0:2]:5683/temperature
 * press reset button and reenable the "Debug Control"
 *
 * to set the body in copper: go to "outbounding"
 *
 * ===================
 * coap-temperature.c
 * ===================
 *  class 5: 
 *    code: coap-temperature.c
 *    date: 2022-09-29
 *    solution: coap-temperature_solution.c
 * ===================
 * What to do: 
 * 
 * Deploy in Cooja two motes:
 *   - border router
 *   - CoAP server
 *
 * In the CoAP server, define a resource that accepts the GET method and returns the temperature value measured by the mote.
 * 
 * Allow plain_text and JSON formats or return NOT_ACCEPTABLE with any other format specified by the client.
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
// CoAP
#include "contiki-net.h"
#include "rest-engine.h"
//----------------------------
// temperature sensor
#if CONTIKI_TARGET_Z1
#include "dev/tmp102.h"
#else
#include "dev/sht11/sht11-sensor.h"
#endif
//----------------------------

////============================
//// UDP setup
////============================
//#define LOCAL_UDP_PORT 1234
//#define REMOTE_UDP_PORT 1234
////----------------------------
//// create connection
//static struct simple_udp_connection conn_coap;
////----------------------------
//// IPv6 address
//static uip_ipaddr_t ipaddr;

//============================
// CoAP setup
//============================
//----------------------------
// GET handler
//----------------------------
// handler (callback function)
void get_handler(void* request, void* response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset) {
    unsigned int accept = -1;
    int temp;
    //Get the format accepted by the client
    REST.get_header_accept(request, &accept);

    if(accept == -1 || accept == REST.type.TEXT_PLAIN){
        // get temperature
        #if CONTIKI_TARGET_Z1
        temp = tmp102.value(TMP102_READ) / 100;
        #else
        temp = (sht11_sensor.value(SHT11_SENSOR_TEMP)/10 - 396) / 10;
        #endif
        //----------------------------
        //Set the content type to plain text
        REST.set_header_content_type(response, REST.type.TEXT_PLAIN);
        //Prepare the response message and store it in buffer, (e.g., using sprintf() or memcpy())
        sprintf((char *) buffer, "Temperature: %d", temp);
        //Set the response payload to the content of buffer, specifying the length of buffer
        REST.set_response_payload(response, buffer, strlen((char *) buffer));
      
    }else if(accept == REST.type.APPLICATION_JSON){
        // get temperature
        #if CONTIKI_TARGET_Z1
        temp = tmp102.value(TMP102_READ) / 100;
        #else
        temp = (sht11_sensor.value(SHT11_SENSOR_TEMP)/10 - 396) / 10;
        #endif
        //----------------------------
        //Set the content type to json
        REST.set_header_content_type(response, REST.type.APPLICATION_JSON);
        //Prepare the response message and store it in buffer, (e.g., using sprintf() or memcpy())
        sprintf((char *) buffer, "{\"temperature\": %d}", temp);
        //Set the response payload to the content of buffer, specifying the length of buffer
        REST.set_response_payload(response, buffer, strlen((char *) buffer));
    }else{
        REST.set_response_status(response, REST.status.NOT_ACCEPTABLE);
        // bad request
        const char *msg = "Bad content type: Support for plain text or json";
        memcpy((char *) buffer, msg, strlen(msg)); 
        REST.set_response_status(response, REST.status.NOT_ACCEPTABLE);
        REST.set_response_payload(response, buffer, strlen((char *) buffer));
    }
}
//----------------------------
// resource
RESOURCE(
    resource_temp, 
    "title=\"Temperature measured by the mote\"; rt=\"temperature\"", 
    get_handler,    // GET
    NULL,           // POST
    NULL,           // PUT
    NULL            // DELETE
);
//----------------------------

//============================
// process setup
//============================
// declare the process
PROCESS(proc_unicast_sender, "Exercise 9: CoAP server");
// start process after Contiki boot
AUTOSTART_PROCESSES(&proc_unicast_sender);
// define the process
PROCESS_THREAD(proc_unicast_sender, ev, data) {
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
    //#if CONTIKI_TARGET_Z1  // set receiver address as Z1 mote as well with ID=2
    //uip_ip6addr(&ipaddr, 0xfe80, 0, 0, 0, 0xc30c, 0, 0, 0x0002);
    //#else  // set receiver address as Sky mote as well with ID=2
    //uip_ip6addr(&ipaddr, 0xfe80, 0, 0, 0, 0x0212, 0x7402, 0x0002, 0x0202);
    //#endif

    // set up the connection
    //simple_udp_register(&conn_coap, LOCAL_UDP_PORT, &ipaddr, REMOTE_UDP_PORT, NULL);
    //--------------------
    // set variables
    //--------------------
    //static int temp;
    //static char message[300];

    rest_init_engine();
    // res_name is the variable name used in RESOURCE
    // "temp" is the URL of the resource
  	rest_activate_resource(&resource_temp, "temp");

	while(1) {
   	   PROCESS_WAIT_EVENT();
	}

    PROCESS_END();
}
