#include "contiki.h"
#include "net/ip/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ip/uip-debug.h"
#include "simple-udp.h"
#include <stdio.h>

#define UDP_PORT 1234

PROCESS(unicast_receiver_process, "Unicast receiver example process");
AUTOSTART_PROCESSES(&unicast_receiver_process);

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
  printf(" on port %d from port %d with length %d: '%s'\n", receiver_port, sender_port, datalen, data);
}

PROCESS_THREAD(unicast_receiver_process, ev, data)
{
  
  static struct simple_udp_connection unicast_connection;
  static uip_ipaddr_t addr;

  PROCESS_BEGIN();

  //sets the address to the sender link-local address
  uip_ip6addr(&addr, 0xfe80, 0, 0, 0, 0x0212, 0x7401, 0x0001, 0x0101);

  simple_udp_register(&unicast_connection, UDP_PORT, &addr, UDP_PORT, receiver);

  while(1) {
    
     PROCESS_WAIT_EVENT(); //infinite wait
  }

  PROCESS_END();
}
