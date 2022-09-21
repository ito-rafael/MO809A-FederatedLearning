#include "contiki.h"

#include "dev/button-sensor.h"
#include "dev/leds.h"
#include "sys/etimer.h"

#include <stdio.h>


PROCESS(button_timer_process, "Button-timer process");

AUTOSTART_PROCESSES(&button_timer_process);

PROCESS_THREAD(button_timer_process, ev, data){
  
  PROCESS_BEGIN();
  
  static struct etimer et;
  etimer_set(&et, CLOCK_SECOND*4);
  SENSORS_ACTIVATE(button_sensor);

  while(1){
     
     PROCESS_WAIT_EVENT();

     leds_toggle(LEDS_ALL);
     
     if(etimer_expired(&et)){
           
		printf("Timer expired\n");
		etimer_reset(&et);
	
     }else if(ev == sensors_event && data == &button_sensor){
		
           printf("Button pressed\n");
     }
  }

  PROCESS_END();
}