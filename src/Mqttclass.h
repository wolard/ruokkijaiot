#ifndef mqtt_h
#define mqtt_h

#include "Arduino.h"
#include <PubSubClient.h>

class Mqttclass

{

private:



  const char* _mqtt_server = "192.168.0.3";
   char _msg[50];

public:  

PubSubClient client;


void reconnect();
void connect();
bool keepalive();
static void callback(char* topic, byte* payload, unsigned int length);

};
#endif