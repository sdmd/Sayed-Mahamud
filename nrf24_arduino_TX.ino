
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
//////////declare CE & CSN PIN
#define CE 9
#define CSN 10 

float voltage;
int sensorValue; 
int i =1;
char charVal[32];  

RF24 radio(CE,CSN);

const byte rxAddr[6] = "00001";

void setup() {
  
  Serial.begin(9600);
  radio.begin();
  radio.setRetries(15, 15);
  radio.openWritingPipe(rxAddr);
   Serial.println("start communication");
  radio.stopListening();
  
}
void loop() {

sensorValue = analogRead(A0);
voltage = sensorValue * (5.0 / 1023.0);

 dtostrf(voltage, 4, 4, charVal);
  for(int i=0;i<sizeof(charVal);i++)
  {
  }

   
   radio.write(&charVal, sizeof(charVal));
  
  Serial.print(i);
  Serial.print(". Voltage ");
  Serial.println(charVal);
  i++;
  delay(5000);

}
