CONTIKI_PROJECT = coap-led.c

all: $(CONTIKI_PROJECT)

CFLAGS += -DUIP_CONF_IPV6=1 -DWITH_UIP6=1 -DUIP_CONF_TCP=0 -DPROJECT_CONF_H=\"project-conf.h\"

CONTIKI = /home/user/contiki

APPS += er-coap

APPS += rest-engine

WITH_UIP6=1
UIP_CONF_IPV6=1

include $(CONTIKI)/Makefile.include
