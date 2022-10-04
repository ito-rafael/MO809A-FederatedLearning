/*
 * ===================
 * coap-led.c
 * ===================
 *  class 5: 
 *    code: coap-led.c
 *    date: 2022-09-29
 *    solution: coap-led_solution.c
 * ===================
 * What to do: 
 * 
 * Deploy in Cooja two motes:
 *   - border router
 *   - CoAP server
 *
 * Define a resource that allows to remotely change the status of LEDs depending on query and body parameters, as follows:
 *
 *   - Query parameter:
 *     color=r|g|b
 *
 *   - Body parameter:
 *     mode=on|off
 *
 * The server returns OK if the request is correct. If instead a parameter is missing or is not as expected, then the server returns BAD_REQUEST.
 */

//============================
// includes
//============================
// general
#include "contiki.h"
#include <stdio.h>
//----------------------------
// LED
#include "dev/leds.h"
//----------------------------
// IPv6
#include "net/ip/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ip/uip-debug.h"
//----------------------------
// CoAP
#include "contiki-net.h"
#include "rest-engine.h"
//----------------------------

//============================
// CoAP setup
//============================

//----------------------------
// PUT handler
//----------------------------

void put_handler(void* request, void* response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset) {
    //----------------------------
    // query parameters
    //----------------------------
    int color_len;
    char color_str[32];
    const char *color = NULL;
    uint8_t led = 0;
    int success = 1;
    // color: "r"||"g"||"b"
    color_len = REST.get_query_variable(request, "color", &color);
    if (color_len == 1) {
        if (color[0] == 'r') {
            led = LEDS_RED;
            sprintf(color_str, "RED");
        } else if (color[0] == 'g') {
            led = LEDS_GREEN;
            sprintf(color_str, "GREEN");
        } else if (color[0] == 'b') {
            led = LEDS_BLUE;
            sprintf(color_str, "BLUE");
        } else {
            success = 0;
            printf("BAD_REQUEST! Color parameter in wrong format!\n");
        }
    }
    printf("Color chosen: %s\n", color_str);
    //----------------------------
    // body parameters
    //----------------------------
    int mode_len;
    const char *mode = NULL;
    mode_len = REST.get_post_variable(request, "mode", &mode);
    // mode: "on"||"off"
    if ( success && (mode_len <= 3) ) {
        if ( strncmp(mode, "on", mode_len) == 0 ) {
            leds_on(led);
        } else if ( strncmp(mode, "off", mode_len) == 0 ) {
            leds_off(led);
        } else {
            success = 0;
            printf("BAD_REQUEST! Mode parameter in wrong format!\n");
        }
    }
    printf("Turning LED %s %s!\n", color_str, mode);
    //----------------------------
    // set request status
    if (success) {
        REST.set_response_status(response, REST.status.OK);
    }
    else {
        REST.set_response_status(response, REST.status.BAD_REQUEST);
    }
}

//----------------------------
// resource
//----------------------------
RESOURCE(
    resource_leds, 
    "title=\"RGB LEDs control\"; rt=\"LEDs\"", 
    NULL,           // GET
    NULL,           // POST
    put_handler,    // PUT
    NULL            // DELETE
);

//============================
// process setup
//============================
// declare the process
PROCESS(proc_coap_server, "Exercise 10: CoAP server");
// start process after Contiki boot
AUTOSTART_PROCESSES(&proc_coap_server);
// define the process
PROCESS_THREAD(proc_coap_server, ev, data) {
    PROCESS_BEGIN();

    //--------------------
    // resources
    //--------------------
    rest_init_engine();
  	rest_activate_resource(&resource_leds, "led");

    //--------------------
    // while loop
    //--------------------
	while(1) {
   	   PROCESS_WAIT_EVENT();
	}

    PROCESS_END();
}
