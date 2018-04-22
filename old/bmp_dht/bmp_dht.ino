/*  Prints values of DHT22 and BMP280 sensors to Serial output. Can be used with
 *  Python script serial_ttyamc0.py to log data in txt file.
 *  Measures humidity, temperature and air pressure.
 *  Setup is done with I2C interface, one of the BMP sensors
 *  needs to be set to 0x76 (setting jumper between GND and SDO).
 *  Andreas Hemmetter, andreas.hemmetter@gmail.com
 *  https://github.com/Ahemmetter/c0bra-hp
 */

// Library
#include <DHT.h>
#include <Wire.h>
//#include <SPI.h>
#include <ThingSpeak.h>
#include <Adafruit_Sensor.h>
#include <bmpnew.h>
#include <Ethernet.h>

// Constants
#define DHTPIN_1 12     // pin for data
//#define DHTPIN_2 11
#define DHTTYPE DHT22   // what type of sensor: DHT 22  (AM2302)

DHT dht1(DHTPIN_1, DHTTYPE); // initializes sensor
//DHT dht2(DHTPIN_2, DHTTYPE); // initializes sensor

byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0xE5, 0xBF};
IPAddress ip(141,76,109,212);
EthernetClient client;
unsigned long myChannelNumber = 115634;
const char * myWriteAPIKey = "266SWBGL2R8WWHN8";

bmpnew bmp; // I2C

// Variables

// --- Setup --- //
void setup()
{
  delay(1000);
  Serial.begin(9600);
  dht1.begin();
  //dht2.begin();
  bmp.begin();
  Ethernet.begin(mac, ip);
  ThingSpeak.begin(client);
}

void loop()
{
  // serial output commands
  
  delay(2000); // reading needs to be delayed by at least 2000ms
  ThingSpeak.setField(1, dht1.readHumidity());
  ThingSpeak.setField(2, dht1.readTemperature());
  //ThingSpeak.setField(3, hum2);
  //ThingSpeak.setField(4, temp2);
  delay(500);
  //ThingSpeak.setField(5, bmp.readTemperature());
  ThingSpeak.setField(6, bmp.readPressure());
  //Serial.println(" Pa");
  //delay(500);
  ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  delay(15000);
  
}
