#include "contiki.h"
#include "net/ip/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ip/uip-debug.h"
#include <stdio.h>

PROCESS(ipv6_process, "IPv6 configuration example process");
AUTOSTART_PROCESSES(&ipv6_process);

static void set_global_address(){
  uip_ipaddr_t ipaddr;
  uip_ip6addr(&ipaddr, 0xabcd, 0, 0, 0, 0, 0, 0, 0);
  uip_ds6_set_addr_iid(&ipaddr, &uip_lladdr);
  uip_ds6_addr_add(&ipaddr, 0, ADDR_AUTOCONF);
}

static void print_addresses(){
  int i;
  printf("IPv6 addresses: ");
  for(i = 0; i < UIP_DS6_ADDR_NB; i++){
    if(uip_ds6_if.addr_list[i].isused){
      uip_debug_ipaddr_print(&uip_ds6_if.addr_list[i].ipaddr);
      printf("\n");
    }
  }
}


PROCESS_THREAD(ipv6_process, ev, data){

  PROCESS_BEGIN();

  set_global_address();
  print_addresses();

  PROCESS_END();
}
