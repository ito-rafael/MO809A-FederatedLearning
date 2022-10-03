#include "contiki.h"
#include "net/ip/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ip/uip-debug.h"
#include "simple-udp.h"
#include <stdio.h>

#define SERVER_UDP_PORT 1234
#define CLIENT_UDP_PORT 4321

PROCESS(unicast_server, "Unicast server");
AUTOSTART_PROCESSES(&unicast_server);

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
  printf(" on port %d from port %d with length %d: %s\n", receiver_port, sender_port, datalen, data);

  printf("Sending echo reply\n");
  simple_udp_sendto(c, data, datalen, sender_addr);
}


PROCESS_THREAD(unicast_server, ev, data)
{
  
  static struct simple_udp_connection unicast_connection;
  static uip_ipaddr_t addr;

  PROCESS_BEGIN();

  simple_udp_register(&unicast_connection, SERVER_UDP_PORT, NULL, CLIENT_UDP_PORT, receiver);

  while(1) {
    
       PROCESS_WAIT_EVENT(); //infinite wait

  }

  PROCESS_END();
}
