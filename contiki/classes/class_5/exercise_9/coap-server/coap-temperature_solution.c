#include <stdio.h>
#include <stdlib.h>
#include "contiki.h"
#include "net/ip/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ip/uip-debug.h"
#include "contiki-net.h"
#include "rest-engine.h"
#if CONTIKI_TARGET_Z1 
#include "dev/tmp102.h"
#else
#include "dev/sht11/sht11-sensor.h"
#endif


void get_handler(void*, void*, uint8_t*, uint16_t, int32_t*);

RESOURCE(temperaturevalue, "title=\"Cooja-simulated temperature\"; rt=\"Temperature value\"", get_handler, NULL, NULL, NULL);

PROCESS(coaptemperature_example, "Erbium Example Server to get temperature value");

AUTOSTART_PROCESSES(&coaptemperature_example);

PROCESS_THREAD(coaptemperature_example, ev, data){

  PROCESS_BEGIN();

  printf("Starting Erbium Example Server\n");

  rest_init_engine();

  rest_activate_resource(&temperaturevalue, "temperature");

  #if CONTIKI_TARGET_Z1
  SENSORS_ACTIVATE(tmp102);
  #else
  SENSORS_ACTIVATE(sht11_sensor);
  #endif
 
  while(1){
    
     PROCESS_WAIT_EVENT();
  }

  PROCESS_END();

}

void get_handler(void* request, void* response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset)
{

  #if CONTIKI_TARGET_Z1
  int temp = tmp102.value(TMP102_READ)/100;
  #else 
  int temp = (sht11_sensor.value(SHT11_SENSOR_TEMP)/10-396)/10;
  #endif	   	

  unsigned int accept = -1;
  REST.get_header_accept(request, &accept);

  if(accept == -1 || accept == REST.type.TEXT_PLAIN){
 
      sprintf((char *) buffer, "Temperature: %d", temp);
      REST.set_header_content_type(response, REST.type.TEXT_PLAIN);
      REST.set_response_payload(response, buffer, strlen((char *) buffer));

  }else if(accept == REST.type.APPLICATION_JSON){

      sprintf((char *) buffer, "{'Temperature':%d}", temp);
      REST.set_header_content_type(response, REST.type.APPLICATION_JSON);
      REST.set_response_payload(response, buffer, strlen((char *) buffer));

  }else{

      const char *msg = "Bad content type: Supporting plain text or json";
      memcpy((char *) buffer, msg, strlen(msg)); 
      REST.set_response_status(response, REST.status.NOT_ACCEPTABLE);
      REST.set_response_payload(response, buffer, strlen((char *) buffer));
  
  }
}
