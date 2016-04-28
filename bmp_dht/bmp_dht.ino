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
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

// Constants
#define DHTPIN_1 5     // pin for data
#define DHTPIN_2 4
#define DHTTYPE DHT22   // what type of sensor: DHT 22  (AM2302)

DHT dht1(DHTPIN_1, DHTTYPE); // initializes sensor
DHT dht2(DHTPIN_2, DHTTYPE); // initializes sensor

Adafruit_BMP280 bm77; // I2C
Adafruit_BMP280 bm76(0x76); // I2C

// Variables
int chk;
float hum1;  // stores humidity
float temp1; // stores temperature
float hum2;  // stores humidity
float temp2; // stores temperature


// --- Setup --- //
void setup()
{
  delay(1000);
  Serial.begin(9600);
  dht1.begin();
  dht2.begin();
  bm77.begin();
  delay(500);
  bm76.begin();
}

void loop()
{
  byte count = 0;
  Wire.begin();
  for (byte i = 1; i < 120; i++)
  {
    Wire.beginTransmission (i);
    if (Wire.endTransmission () == 0)
      {
      Serial.print ("Found address: ");
      Serial.print (i, DEC);
      Serial.print (" (0x");
      Serial.print (i, HEX);
      Serial.println (")");
      count++;
      delay (1);  // maybe unneeded?
      } // end of good response
  } // end of for loop
  Serial.print ("Found ");
  Serial.print (count, DEC);
  Serial.println (" device(s).");
  
  hum1 = dht1.readHumidity(); // reads and stores hum
  temp1= dht1.readTemperature(); // reads and stores temp
  hum2 = dht2.readHumidity(); // reads and stores hum
  temp2= dht2.readTemperature(); // reads and stores temp
  // serial output commands
  
  delay(2000); // reading needs to be delayed by at least 2000ms
  Serial.print("DHT22_1: ");
  Serial.print("hum1 = ");
  Serial.print(hum1);
  Serial.print(" %, temp1 = ");
  Serial.print(temp1);
  Serial.println(" C");

  Serial.print("DHT22_2: ");
  Serial.print("hum2 = ");
  Serial.print(hum2);
  Serial.print(" %, temp2 = ");
  Serial.print(temp2);
  Serial.println(" C");
  delay(500);
  Serial.print("BMP280_0x77: ");
  Serial.print("t_77 = ");
  Serial.print(bm77.readTemperature());
  Serial.print(" C");
  Serial.print("p_77 = ");
  Serial.print(bm77.readPressure());
  Serial.println(" Pa");
  delay(500);
  Serial.print("BMP280_0x76: ");
  Serial.print("t_76 = ");
  Serial.print(bm76.readTemperature());
  Serial.print(" C");
  Serial.print("p_76 = ");
  Serial.print(bm76.readPressure());
  Serial.println(" Pa");
  
  Serial.println();
  
}
