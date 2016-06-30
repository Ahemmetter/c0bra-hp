#include <Adafruit_BMP280.h>

/*
Temperatur and Humidity Sensor Monitor for C0BRA @ LNGS
 
 * Created 24.11.2012
 * By Thomas Goepfert <thomas.goepfert@tu-dresden.de>
 * 
 * modified 08Feb2013
 * - BPM085 uses V3.3 logic, depending on the cable length it sometimes doesn't work
 * - hardware pull-ups (4.7k) to 3.3 on SDA, SCL and all XCLR
 * - keep (in addition) internal pull-up on SDA, SCL (most reliable performance so far)
 * - watchdog: reset if not in loop() after 2s
 */

#include <Wire.h>

// Ethernet
#include <Arduino.h>
#include <HardwareSerial.h>
#include <SPI.h>

float temp;
float bar;

Adafruit_BMP280 bmp;

void setup() {

  //disable SD Card for now
  /*pinMode(SCL, OUTPUT);
  digitalWrite(SCL, LOW);
  pinMode(SDA, INPUT);
  digitalWrite(SDA, HIGH);*/
  delay(20);

  Serial.begin(9600);

  bmp.begin();

}

void loop() {
  delay(2000);
  temp = bmp.readTemperature();
  bar = bmp.readPressure();
  Serial.println(temp);
  Serial.println(bar);

}
