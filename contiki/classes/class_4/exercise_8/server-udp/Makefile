CONTIKI_PROJECT = unicast-server.c

all: $(CONTIKI_PROJECT)

CFLAGS += -DUIP_CONF_IPV6=1 -DWITH_UIP6=1 -DPROJECT_CONF_H=\"project-conf.h\"

CONTIKI = /home/user/contiki

WITH_UIP6=1
UIP_CONF_IPV6=1

include $(CONTIKI)/Makefile.include
