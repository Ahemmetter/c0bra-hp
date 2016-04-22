/*  Prints values of DHT22 sensor to Serial output. Can be used with
 *  Python script serial_ttyamc0.py to log data in txt file.
 *  Measures humidity and temperature.
 *  Andreas Hemmetter, andreas.hemmetter@gmail.com
 *  https://github.com/Ahemmetter/c0bra-hp
 */

// Library
#include <DHT.h>

// Constants
#define DHTPIN 2     // pin for data
#define DHTTYPE DHT22   // what type of sensor: DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); // initializes sensor

// Variables
int chk;
float hum;  // stores humidity
float temp; // stores temperature

// --- Setup --- //
void setup()
{
  Serial.begin(9600);
  dht.begin();
}

// --- Loop --- //
void loop()
{
  hum = dht.readHumidity(); // reads and stores hum
  temp= dht.readTemperature(); // reads and stores temp
  // serial output commands
  Serial.print("Humidity: ");
  Serial.print(hum);
  Serial.print(" %, Temp: ");
  Serial.print(temp);
  Serial.println(" Celsius");
  delay(2000); // reading needs to be delayed by at least 2000ms
}
