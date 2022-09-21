#include <stdio.h>
#include "dev/button-sensor.h"
#include "dev/leds.h"
#include "sys/etimer.h"

PROCESS(game_process, "Game Process");

AUTOSTART_PROCESSES(&game_process);

PROCESS_THREAD(game_process, ev, data){

   PROCESS_BEGIN();

   static struct etimer five_timer;
   static struct etimer half_timer;
   static int total = 0;
   static int score = 0;

   etimer_set(&five_timer, 5*CLOCK_SECOND);
   SENSORS_ACTIVATE(button_sensor);

   while(1){

     PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&five_timer));

     leds_toggle(LEDS_GREEN);
	 total += 1;
	 etimer_set(&half_timer, CLOCK_SECOND*0.5);

	 PROCESS_WAIT_EVENT();

	 if(ev == sensors_event && data == &button_sensor)
         score += 1;
	 printf("Your score is %d out of %d\n", score, total);
      etimer_reset(&five_timer);
   }

   PROCESS_END();
}
