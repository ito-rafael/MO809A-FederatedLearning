#include "contiki.h"
#include "sys/etimer.h"
#include "lib/random.h"
#include <stdio.h>
#include "net/ip/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ip/uip-debug.h"
#include "simple-udp.h"
#include "net/rpl/rpl.h" //to have access to the RPL features

#define UDP_PORT 1234
#define PRINT_TIME (random_rand() % (5 * CLOCK_SECOND))


PROCESS(rpl_root_process, "RPL root and receiver process");
AUTOSTART_PROCESSES(&rpl_root_process);

//set the global address of the RPL root and therefore the global prefix of the RPL network, to be communicated by the root to all the nodes
static void set_global_address(){
  
  static uip_ipaddr_t ipaddr;

  uip_ip6addr(&ipaddr, 0xabcd, 0, 0, 0, 0, 0, 0, 0);
  uip_ds6_set_addr_iid(&ipaddr, &uip_lladdr);
  uip_ds6_addr_add(&ipaddr, 0, ADDR_AUTOCONF);

  // Initialize the RPL module as ROOT node
  rpl_dag_t *dag;
  dag = rpl_set_root(RPL_DEFAULT_INSTANCE,(uip_ip6addr_t *)&ipaddr);
  uip_ip6addr(&ipaddr, 0xabcd, 0, 0, 0, 0, 0, 0, 0);
  //set the prefix of the dag to be the first 64 bits of ipaddr
  rpl_set_prefix(dag, &ipaddr, 64);

}

static void print_addresses(){ //Print the addresses of the interface
  int i;
  printf("IPv6 addresses: ");
  for(i = 0; i < UIP_DS6_ADDR_NB; i++) {
    if(uip_ds6_if.addr_list[i].isused) {
      uip_debug_ipaddr_print(&uip_ds6_if.addr_list[i].ipaddr);
      printf("\n");
    }
  }
}

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


PROCESS_THREAD(rpl_root_process, ev, data)
{
  static struct etimer print_timer;
  static struct simple_udp_connection unicast_connection;

  PROCESS_BEGIN();

  set_global_address();

  simple_udp_register(&unicast_connection, UDP_PORT, NULL, UDP_PORT, receiver);

  etimer_set(&print_timer, PRINT_TIME);
  
  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&print_timer));
       print_addresses();
       etimer_reset(&print_timer);
  }  

  PROCESS_END();
}
