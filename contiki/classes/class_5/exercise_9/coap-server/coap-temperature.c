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
static int* temp;
#if CONTIKI_TARGET_Z1
#include "dev/tmp102.h"
#else
#include "dev/sht11/sht11-sensor.h"
#endif

//----------------------------
// fn to measure temperature
//----------------------------
void get_temp() {
    int temp_buffer;
    // get temperature
    #if CONTIKI_TARGET_Z1
    temp_buffer = tmp102.value(TMP102_READ) / 100;
    #else
    temp_buffer = (sht11_sensor.value(SHT11_SENSOR_TEMP)/10 - 396) / 10;
    #endif
    //temp_buffer = 25;
    temp = &temp_buffer;
}

//============================
// CoAP setup
//============================

//----------------------------
// GET handler
//----------------------------
// handler (callback function)
void get_handler(void* request, void* response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset) {
    unsigned int accept = -1;
    // get the format accepted by the client
    REST.get_header_accept(request, &accept);
    //----------------------------
    // text/plain
    //----------------------------
    if(accept == -1 || accept == REST.type.TEXT_PLAIN){
        get_temp(temp);
        // set the content type to plain text
        REST.set_header_content_type(response, REST.type.TEXT_PLAIN);
        // prepare response message and store it in buffer
        sprintf((char *) buffer, "Temperature: %d", *temp);
        // set response payload to the content of buffer (needs to specify buffer length)
        REST.set_response_payload(response, buffer, strlen((char *) buffer));
    }
    //----------------------------
    // application/json
    //----------------------------
    else if(accept == REST.type.APPLICATION_JSON){
        get_temp(temp);
        // set the content type to json
        REST.set_header_content_type(response, REST.type.APPLICATION_JSON);
        // prepare response message and store it in buffer
        sprintf((char *) buffer, "{\"temperature\": %d}", *temp);
        // set response payload to the content of buffer (needs to specify buffer length)
        REST.set_response_payload(response, buffer, strlen((char *) buffer));
    }
    //----------------------------
    // bad request
    //----------------------------
    else{
        REST.set_response_status(response, REST.status.NOT_ACCEPTABLE);
        const char *msg = "Bad content type: support for plain text or json only";
        memcpy((char *) buffer, msg, strlen(msg)); 
        REST.set_response_status(response, REST.status.NOT_ACCEPTABLE);
        REST.set_response_payload(response, buffer, strlen((char *) buffer));
    }
}

//----------------------------
// resource
//----------------------------
RESOURCE(
    resource_temp, 
    "title=\"Temperature measured by the mote\"; rt=\"temperature\"", 
    get_handler,    // GET
    NULL,           // POST
    NULL,           // PUT
    NULL            // DELETE
);

//============================
// process setup
//============================
// declare the process
PROCESS(proc_coap_server, "Exercise 9: CoAP server");
// start process after Contiki boot
AUTOSTART_PROCESSES(&proc_coap_server);
// define the process
PROCESS_THREAD(proc_coap_server, ev, data) {
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
    // resources
    //--------------------
    rest_init_engine();
  	rest_activate_resource(&resource_temp, "temp");

    //--------------------
    // while loop
    //--------------------
	while(1) {
   	   PROCESS_WAIT_EVENT();
	}

    PROCESS_END();
}
