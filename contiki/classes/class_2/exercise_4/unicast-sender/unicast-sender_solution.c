#include "contiki.h"
#include "sys/etimer.h"
#include "lib/random.h"
#include "net/ip/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ip/uip-debug.h"
#include "simple-udp.h"
#include <stdio.h>
#include <string.h>

#define UDP_PORT 1234
#define SEND_TIME (random_rand() % (5 * CLOCK_SECOND))


PROCESS(unicast_sender_process, "Unicast sender example process");
AUTOSTART_PROCESSES(&unicast_sender_process);

PROCESS_THREAD(unicast_sender_process, ev, data)
{
  static struct etimer send_timer;
  static uip_ipaddr_t addr;
  static struct simple_udp_connection unicast_connection;
  static unsigned int message_number; //message counter

  PROCESS_BEGIN();
   
  //sets the address to the receiver link-local address
  uip_ip6addr(&addr, 0xfe80, 0, 0, 0, 0x0212, 0x7402, 0x0002, 0x0202);

  simple_udp_register(&unicast_connection, UDP_PORT, &addr, UDP_PORT, NULL);

  message_number = 0;
  char buffer[300];
  etimer_set(&send_timer, SEND_TIME);

  while(1) {

    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&send_timer));
    etimer_reset(&send_timer);

    printf("Sending unicast message to ");
    uip_debug_ipaddr_print(&addr);
    printf("\n");
    sprintf(buffer, "Message number %d", message_number);

    message_number++;
    simple_udp_send(&unicast_connection, buffer, strlen(buffer));
  }

  PROCESS_END();
}
