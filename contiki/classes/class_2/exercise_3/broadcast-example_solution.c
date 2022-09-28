#include "contiki.h"
#include "sys/etimer.h"
#include "lib/random.h"
#include <stdio.h>
#include "net/ip/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ip/uip-debug.h"
#include "simple-udp.h"
#if CONTIKI_TARGET_Z1
#include "dev/tmp102.h"
#else
#include "dev/sht11/sht11-sensor.h"
#endif

#define UDP_PORT 1234
#define SEND_TIME (random_rand() % (5 * CLOCK_SECOND))


PROCESS(broadcast_example_process, "UDP broadcast example process");
AUTOSTART_PROCESSES(&broadcast_example_process);

static void receiver(struct simple_udp_connection *c,
         const uip_ipaddr_t *sender_addr,
         uint16_t sender_port,
         const uip_ipaddr_t *receiver_addr,
         uint16_t receiver_port,
         const uint8_t *data,
         uint16_t datalen)
{
   printf("Data received from ");
   uip_debug_ipaddr_print(sender_addr);
   printf(" on port %d from port %d with length %d. The sensed temperature is: %d degrees centigrade\n", receiver_port, sender_port, datalen, *data);
}

PROCESS_THREAD(broadcast_example_process, ev, data)
{
  int temp;
  static struct etimer send_timer;
  static struct simple_udp_connection broadcast_connection;
  static uip_ipaddr_t addr;

  PROCESS_BEGIN();

  simple_udp_register(&broadcast_connection, UDP_PORT, NULL, UDP_PORT, receiver);
  uip_create_linklocal_allnodes_mcast(&addr);
  #if CONTIKI_TARGET_Z1
  SENSORS_ACTIVATE(tmp102);
  #else
  SENSORS_ACTIVATE(sht11_sensor);
  #endif
  etimer_set(&send_timer, SEND_TIME);
  
  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&send_timer));
    printf("Sensing temperature\n");
    #if CONTIKI_TARGET_Z1
    temp = tmp102.value(TMP102_READ)/100;
    #else
    temp = (sht11_sensor.value(SHT11_SENSOR_TEMP)/10-396)/10;
    #endif
    printf("Sending broadcast\n");
    simple_udp_sendto(&broadcast_connection, &temp, sizeof(temp), &addr);
    etimer_reset(&send_timer);
  }

  PROCESS_END();
}
