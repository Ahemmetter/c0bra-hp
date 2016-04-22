/*
Temperatur and Humidity Sensor Monitor
 
 * Created 21.02.2012
 * By Thomas Goepfert
 * 
 
 */

//----Packages----//

// (Ethernet and Pachube, login: iktp_cobra, lowbackground)
#include <Arduino.h>
#include <HardwareSerial.h>

//DHTLib
#include <dhtlib.h> //http://arduino.cc/playground/Main/DHTLib

// great tool
#include <MemoryFree.h>
#include <avr/pgmspace.h>

//-- DHT setup --//
#define DHT22_PIN  2

dht DHT;
float dhtValues[1];
int chk;

//-- LED setup --//
#define LED_RED 8
#define LED_YELLOW 9
#define LED_GREEN 7

//-----------------------------------------------------------------//
void setup(void)
{
  //disable SD Card for now
  pinMode(4, OUTPUT);
  digitalWrite(4,HIGH);

  Serial.begin(9600);
  digitalWrite(LED_YELLOW, HIGH);
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, LOW);
  //sensors.begin();
  
}


//-----------------------------------------------------------------//
void loop(void) { 


  digitalWrite(LED_GREEN, LOW);
  requestDHT();
  printDHT();
  delay(2000);
  digitalWrite(LED_GREEN, HIGH);
  
}


//-----------------------------------------------------------------//
void requestDHT() {

  chk = DHT.read22(DHT22_PIN);
  dhtValues[0] = DHT.temperature;
  dhtValues[1] = DHT.humidity;

}

//-----------------------------------------------------------------//
void printDHT(){

  Serial.print("DHT22 A: "); 
  switch (chk)
  // catches sensor errors
  {
  case 0:  
    Serial.print("OK, "); 
    break;
  case -1: 
    Serial.print("Checksum error, "); 
    break;
  case -2: 
    Serial.print("Time out error, "); 
    break;
  default: 
    Serial.print("Unknown error, "); 
    break;
  }

  Serial.print("at T = ");
  Serial.print(dhtValues[0], 1);
  Serial.print(" C and rH = ");
  Serial.print(dhtValues[1], 1);
  Serial.println(" %");
  
}


