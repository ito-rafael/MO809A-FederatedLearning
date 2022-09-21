/*
 * ===================
 *  Exercise 2
 * ===================
 * Write a program that:
 *   - with a period of 5 seconds, toggles the green LED and give    s the user 0.5 seconds to press the button;
 *   - every time the user succeeds, his score is increased by 1;                                               /
 *   - in both cases (either the user succeeds or not), the progr    am prints out “Your score is M out of N”.
 *   - Run the program in Cooja.
 */

#include "contiki.h"
#include "sys/etimer.h"
#include "dev/leds.h"
#include "dev/button-sensor.h"
#include <stdio.h>

// declare the process
PROCESS(led_button_score, "exercise_2");
// start process after Contiki boot
AUTOSTART_PROCESSES(&led_button_score);

// define the process
PROCESS_THREAD(led_button_score, ev, data) {

    PROCESS_BEGIN();

    // etimer
    static struct etimer timer_five;
    static struct etimer timer_half;
    // set timers
    etimer_set(&timer_five, 5*CLOCK_SECOND);

    // button
    SENSORS_ACTIVATE(button_sensor);

    // score
    static int score = 0;
    static int attempts = 0;

    while(1) {
        // start 0.5 s timer
        etimer_set(&timer_half, 0.5*CLOCK_SECOND);
        // toggle green LED
        leds_toggle(LEDS_GREEN);
        // wait 0.5 s timer
        while (!etimer_expired(&timer_half)) {
            PROCESS_WAIT_EVENT();
            // button pressed
            if (ev == sensors_event && data == &button_sensor) {
                score += 1;
            }
        };
        // wait 5 s timer expire
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer_five));
        etimer_reset(&timer_five);
        attempts += 1;
        printf("Your score is %d out of %d\n", score, attempts);
    }

    PROCESS_END();
}
