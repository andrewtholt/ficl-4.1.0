#include <stdio.h>

#include <mosquitto.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

struct cbMqttMessage {
    uint16_t msgFlag;
    char topic[64];
    char payload[32];
} ;

uint32_t sizeOfMessage() {
    return ( sizeof( struct cbMqttMessage ));
}

void messageCallback(struct mosquitto *mosq, void *obj,const struct mosquitto_message *message) { 
//    printf("messageCallback\n");

    ((struct cbMqttMessage *)obj)->msgFlag=0xffff;

    strcpy(((struct cbMqttMessage *)obj)->topic,(char *)message->topic);
    strcpy( ((struct cbMqttMessage *)obj)->payload,(char *)message->payload);

}


