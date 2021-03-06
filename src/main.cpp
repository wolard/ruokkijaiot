

#include <Arduino.h>
#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <EEPROM.h>
#include <PubSubClient.h>
#include <Mqttclass.h>
#include <ArduinoJson.h>
#include "Shelf.h"
 
const char ssid[] = "TP-LINK_BF33";  //  your network SSID (name)
const char pass[] = "82447410";       // your network password
const char* mqtt_server = "192.168.0.3";
const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets
// NTP Servers:
const char* ntpServerName = "fi.pool.ntp.org";
IPAddress timeServer;


//IPAddress timeServer(132, 163, 4, 101); // time-a.timefreq.bldrdoc.gov
// IPAddress timeServer(132, 163, 4, 102); // time-b.timefreq.bldrdoc.gov
// IPAddress timeServer(132, 163, 4, 103); // time-c.timefreq.bldrdoc.gov
//mqtt192.168.0.3

const int timeZone = 2;     // Central European Time
//const int timeZone = -5;  // Eastern Standard Time (USA)
//const int timeZone = -4;  // Eastern Daylight Time (USA)
//const int timeZone = -8;  // Pacific Standard Time (USA)
//const int timeZone = -7;  // Pacific Daylight Time (USA)
//

WiFiUDP Udp;
WiFiClient espClient;
Mqttclass mqttparser;

unsigned int localPort = 8888;  // local port to listen for UDP packets

void printDigits(int digits){
  // utility for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

static void digitalClockDisplay(){
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  //printDigits(second());

}

void sendNTPpacket(IPAddress &address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}

time_t getNtpTime()
{
  while (Udp.parsePacket() > 0) ; // discard any previously received packets
  Serial.println("Transmit NTP Request");
    Serial.println(timeServer.toString());
  sendNTPpacket(timeServer);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Serial.println("Receive NTP Response");
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];

return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
getNtpTime();

}




void setup() {
 DynamicJsonBuffer jsonBuffer;
 String input =
      "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";
  JsonObject& root = jsonBuffer.parseObject(input);
  String output;
  root.printTo(output);
Feedtime shelf1time;
  shelf1time.Hours=16;
shelf1time.Minutes=30;

Shelf shelf1(D3,&shelf1time);
 shelf1.InitShelf();
Feedtime shelf2time;
  shelf2time.Hours=12;
shelf2time.Minutes=00;
 

 Serial.begin(9600);
  EEPROM.begin(512);
  delay(2000);

 shelf1.InitShelf();
 Feedtime eepromtime;;

 
   int shelf1Address=0;
 int shelf2Address;

 shelf2Address += sizeof(shelf1time);
//EEPROM.put(shelf2Address,shelf2time);
//EEPROM.commit();
 Serial.println(analogRead(A0));
 EEPROM.get(shelf2Address, eepromtime);
   Serial.println("from eeprom");
 Serial.println(eepromtime.Hours);
  Serial.println(eepromtime.Minutes);
Serial.println("next eeprom address");
//Serial.println(eeAddress);
  Serial.println(output);
  Serial.println("TimeNTP Example");
  Serial.print("Connecting to ");
  
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
 
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }


  Serial.print("IP number assigned by DHCP is ");
  Serial.println(WiFi.localIP());
  Serial.println("Starting UDP");
WiFi.hostByName(ntpServerName, timeServer);
  Udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(Udp.localPort());
  Serial.println("waiting for sync");


 mqttparser.client.setClient(espClient);
   mqttparser.client.setServer(mqtt_server, 1883);
  mqttparser.client.setCallback(mqttparser.callback);
 time_t Syncint = 0;
 setSyncInterval(16000000);
 setSyncProvider(getNtpTime);
delay(1000);


}
time_t prevDisplay = 0; // when the digital clock was displayed
void loop() {
  if (timeStatus() != timeNotSet) {    
   if (now() != prevDisplay) { //update the display only if time has changed
     prevDisplay = now();
     digitalClockDisplay();
     int val = analogRead(0);
  val = map(val, 550, 680, 0, 100);
  if (val<=0)
  {
    val=0;
  }
  else if (val>100)
  {
    val=100;
  }
 char msgBuffer[10];           // make sure this is big enough to hold your string
    char* percentage;

    

    percentage = dtostrf(val, 3, 0, msgBuffer);


  
  //shelf1.OpenShelf();
   mqttparser.client.publish("ruokkija", percentage);

   }
 }
 if (!mqttparser.client.connected()) {
    mqttparser.reconnect();
  }
  mqttparser.client.loop();


}
