#include <SPI.h>
#include "nRF24L01.h"
#include <RF24.h>
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/************************* Adafruit.io Setup *********************************/
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "....."
#define AIO_KEY         "................."
/************ Global State (you don't need to change this!) ******************/
//uint32_t x=0;
#define CE D1
#define CSN D2
char text[32];
int i =1;
const byte rxAddr[6] = "00001";
//////////////////////////////////////////////////////////////////
WiFiClient client;
RF24 radio(CE,CSN);
//////////////////////////////////////////////////////////////////
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/
//Adafruit_MQTT_Publish photocell = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/gauge");

Adafruit_MQTT_Publish volt = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/gauge");

/*************************** Sketch Code ************************************/

void MQTT_connect();

void setup() {
  Serial.begin(9600);
  delay(10);

WiFi.mode(WIFI_STA);
  /* start SmartConfig */
  WiFi.beginSmartConfig();

  /* Wait for SmartConfig packet from mobile */
  Serial.println("Waiting for SmartConfig.");
  while (!WiFi.smartConfigDone()) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("SmartConfig done.");

  /* Wait for WiFi to connect to AP */
  Serial.println("Waiting for WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi Connected.");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
/*-----------------------------------------------*/
  radio.begin();
  radio.openReadingPipe(0, rxAddr);
  radio.startListening();
 Serial.println("start receiving "); 
}



void loop() {
  
  if (radio.available())
  {
    text[32]={0};
    radio.read(&text, sizeof(text)); 
  Serial.println("connection OK");
   
    MQTT_connect();
  Serial.print("...");
  if (! volt.publish(text)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
        Serial.print(i);
    Serial.print(". Voltage ");
    Serial.println(text);
    i++;
  }
  }
  else {  Serial.println("connection LoST !");
    }

delay(5000);
}



void MQTT_connect() {
  int8_t ret;
  if (mqtt.connected()) {
    return;
  }
  Serial.print("Connecting to MQTT... ");
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
