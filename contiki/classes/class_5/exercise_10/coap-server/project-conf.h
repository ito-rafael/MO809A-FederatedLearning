// neighbor discovery
#undef UIP_CONF_ND6_SEND_NA
#define UIP_CONF_ND6_SEND_NA 1

//reduce RAM consumption when using CoAP
#ifndef PROJECT_CONF_H_
#define PROJECT_CONF_H_

//Set the max response payload before fragmentation
#undef REST_MAX_CHUNK_SIZE 
#define REST_MAX_CHUNK_SIZE 64

//Set the max number of concurrent transactions that the node can handle
#undef COAP_MAX_OPEN_TRANSACTIONS 
#define COAP_MAX_OPEN_TRANSACTIONS 4

//Set the max number of entries in neighbors table
#undef NBR_TABLE_CONF_MAX_NEIGHBORS 
#define NBR_TABLE_CONF_MAX_NEIGHBORS 10

//Set the max number of routes handled by the node
#undef UIP_CONF_MAX_ROUTES 
#define UIP_CONF_MAX_ROUTES 10

// Set the amount of memory reserved to the uIP packet buffer
#undef UIP_CONF_BUFFER_SIZE 
#define UIP_CONF_BUFFER_SIZE 280

#endif
