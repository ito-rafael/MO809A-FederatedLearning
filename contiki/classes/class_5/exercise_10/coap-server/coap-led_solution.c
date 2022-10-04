#include <stdio.h>
#include <stdlib.h>
#include "contiki.h"
#include "net/ip/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ip/uip-debug.h"
#include "contiki-net.h"
#include "rest-engine.h"
#include "dev/leds.h"

void leds_handler(void* request, void* response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset){

  int len;
  const char *color = NULL;
  const char *mode = NULL;
  uint8_t led = 0;
  int success = 1;

  if ((len = REST.get_query_variable(request, "color", &color))){

    printf("Color: %s\n", color);

    if(strncmp(color, "r", len) == 0){
      led = LEDS_RED;
    }else if(strncmp(color, "g", len) == 0){
      led = LEDS_GREEN;
    }else if(strncmp(color, "b", len) == 0){
      led = LEDS_BLUE;
    }else{ //the provided query parameter is not admitted (must be r || g || b)
      success = 0;
    }

  }else{ //no query parameter was provided
    success = 0;
  }

  if(success && (len = REST.get_post_variable(request, "mode", &mode))){

    printf("Mode: %s\n", mode);

    if(strncmp(mode, "on", len) == 0){
      leds_on(led);
    }else if(strncmp(mode, "off", len) == 0){
      leds_off(led);
    }else{ //the provided body parameter is not admitted (must be on || off)
      success = 0;
    }
  }else{ //no body parameter was provided
    success = 0;
  }

  if(!success){
    REST.set_response_status(response, REST.status.BAD_REQUEST);
  }else{
    REST.set_response_status(response, REST.status.OK);
  }


}


RESOURCE(leds, "title=\"Change the status of LEDs\";rt=\"LEDs\"",  NULL, NULL, leds_handler,  NULL);


PROCESS(leds_server_example, "Erbium Example Server for controlling LEDs");


AUTOSTART_PROCESSES(&leds_server_example);


PROCESS_THREAD(leds_server_example, ev, data){

  PROCESS_BEGIN();

  printf("Starting Erbium Example Server\n");

  rest_init_engine();

  rest_activate_resource(&leds, "actuators/leds");

  while(1) {

    PROCESS_WAIT_EVENT();

  }

  PROCESS_END();

}
