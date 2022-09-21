/*
 * ===================
 *  Exercise 1
 * ===================
 * Write a program that loops indefinitely and waits for an event.
 * If the button has been pressed, toggles LEDs and prints out “Button pressed”.
 * If, instead, the timer has expired, toggles LEDs and prints out “Timer expired”. 
 * Run the program in Cooja.
 */

#include "contiki.h"
#include "sys/etimer.h"
#include "dev/leds.h"
#include "dev/button-sensor.h"
#include <stdio.h>
//LEDS_BLUE LEDS_RED LEDS_GREEN LEDS_ALL

// declare the process
PROCESS(toggle_leds_timer, "exercise_1");
// start process after Contiki boot
AUTOSTART_PROCESSES(&toggle_leds_timer);

// define the process
PROCESS_THREAD(toggle_leds_timer, ev, data) {
    
    PROCESS_BEGIN();
    
    // etimer
    static struct etimer et;
    etimer_set(&et, 5*CLOCK_SECOND);
    // button
    SENSORS_ACTIVATE(button_sensor);
    
    while(1) {
        // wait event
        PROCESS_WAIT_EVENT();
        // button pressed
        if (ev == sensors_event && data == &button_sensor) {
            printf("Button pressed\n");
        }
        // timer expired
        else if (etimer_expired(&et)) {
            printf("Timer expired\n");
            //reset timer
            etimer_reset(&et);
        };
        // toggle LEDs
        leds_toggle(LEDS_ALL);
    }

    PROCESS_END();
}
