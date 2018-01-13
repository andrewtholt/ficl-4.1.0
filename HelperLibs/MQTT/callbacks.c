#include <mosquitto.h>
#include <stdint.h>
#include <stdbool.h>

struct cbMqttMessage {
    uint8_t msgFlag;
    char topic[64];
    char payload[32];
} ;

uint32_t sizeOfMessage() {
    return ( sizeof( struct cbMqttMessage ));
}

void messageCallback(struct mosquitto *mosq, void *obj,const struct mosquitto_message *message) { 
}


