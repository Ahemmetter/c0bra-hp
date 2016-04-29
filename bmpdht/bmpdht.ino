/*  Prints values of DHT22 and BMP280 sensors to Serial output.
 *  Can be used with Python script serial_ttyamc0.py to log data in txt file.
 *  Measures humidity, temperature (twice) and air pressure.
 *  Setup is done with SPI interface, 10k pullups are placed on SCK, SDO and CS lines.
 *  
 *  LED patterns:
 *  --------------------------------------------------------
 *  YELLOW on             : power, Serial connection
 *  GREEN blinks 2x       : 2 measurements (#1 DHT, #2 BMP)
 *  GREEN blink, then RED : BMP sensor not returning values
 *  RED blink, then GREEN : DHT not returning values
 *  RED on                : either sensor not connected
 *  RED blinks 3x         : reboot
 *  
 *  Watchdog counts till 8 seconds and reboots Arduino board, unless it has been
 *  reset. This catches infinite loops and general problems.
 *  
 *  Pullups seem to be not necessary in either case. LEDs are connected to a
 *  390 Ohm resistor each.
 *  
 *  Tested with Arduino Uno SMD R3 board and Arduino IDE 1.6.8 running on Ubuntu 14.04
 *  
 *  Last edit: April 29, 2016
 *  
 *  Andreas Hemmetter, andreas.hemmetter@gmail.com
 *  https://github.com/Ahemmetter/c0bra-hp
 */

// --- Libraries --- //
#include <DHT.h>                    // DHT library for DHT22
#include <avr/wdt.h>                // Watchdog, interrupts and reboots     
#include <Adafruit_BMP280.h>        // BMP library for BMP280

// --- Constants --- //
// DHT22
#define DHTPIN 5                    // pin for DHT data

// BMP280 (SPI)
#define BMP_SCK 13                  // Serial clock pin for BMP (10k pullup to +5V)
#define BMP_MISO 12                 // SDO, has 10k pullup
#define BMP_MOSI 11                 // SDI, no pullup
#define BMP_CS 10                   // CS, has 10k pullup

// LEDs
#define RED 2                       // pin for LEDs
#define YELLOW 3
#define GREEN 4

// --- Variables --- //
float bmp_temp;                     // values from sensors
float bmp_press;
float dht_temp;
float dht_hum;
int chk;                            // DHT status (0 or 1)
int k;                              // counter variable

// --- Initialization --- //
DHT dht(DHTPIN, DHT22);             // initializes sensors
Adafruit_BMP280 bmp(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK);

// --- Setup --- //
void setup() {
  wdt_disable();                    // disables watchdog for initial communication
  pinMode(YELLOW, OUTPUT);          // LED pins set as output
  pinMode(RED, OUTPUT);
  digitalWrite(YELLOW, HIGH);       // starts yellow LED
  Serial.begin(9600);               // begin serial connection with 9600 baud
  delay(500);                       // delay to get connection and sensors ready
  wdt_enable(WDTO_8S);              // start watchdog timeout (4 sec might be too short)

  for (k = 1; k <= 3; k = k + 1) {
    // blinks 3x to show that Arduino rebooted
    digitalWrite(RED, HIGH);
    delay(100);
    digitalWrite(RED, LOW);
    delay(100);
  }

  dht.begin();                      // starts DHT sensor
  chk = dht.read(DHTPIN);           // reads DHT status
  
  while ((!bmp.begin()) || (chk != 1) ) {
    // checks DHT and BMP status and shows red LED, in case one of them
    // wasnt found. Loops and resets timeout each time.
    wdt_reset();
    Serial.println("Could not find a valid sensor, check wiring!");
    digitalWrite(RED, HIGH);
    delay(300);
    digitalWrite(RED, LOW);
    delay(1000);
    chk = dht.read(DHTPIN);
  }
}

// --- Loop --- //
void loop() {
  wdt_reset();                      // resets watchdog
  pinMode(GREEN, OUTPUT);           // gets green LED ready
  digitalWrite(GREEN, LOW);
  delay(1600);                      // delay for sensors to be read. In total, at least 2 sec!
  
  chk = dht.read(DHTPIN);           // checks if connection to DHT sensor is ok (0: no connection, 1: ok)
  if (chk == 1) {
    dht_temp = dht.readTemperature();
    dht_hum = dht.readHumidity();   // reads DHT sensor
    digitalWrite(GREEN, HIGH);      // blinks once for successful DHT sensor values
    Serial.print("DHT22: ");        // prints values (hum and temp)
    Serial.print("hum = ");
    Serial.print(dht_hum);
    Serial.print(" %, temp = ");
    Serial.print(dht_temp);
    Serial.println(" C");
    delay(150);
    digitalWrite(GREEN, LOW);
    delay(100);
  }
  else {
    // if no connection, blink red instead and print "nan"
    // (keeps formatting ok)
    digitalWrite(RED, HIGH);
    Serial.print("DHT22: ");
    Serial.print("hum = ");
    Serial.print(dht_hum);
    Serial.print(" %, temp = ");
    Serial.print(dht_temp);
    Serial.println(" C");
    delay(150);
    digitalWrite(RED, LOW);
    delay(100);
  }

  bmp_temp = bmp.readTemperature(); // reads BMP sensor
  bmp_press = bmp.readPressure();
  
  if (bmp_press >= 0.0) {
    // if sensor isnt attached, it shows 0.0 or negative pressure.
    // This is used as check criterion
    digitalWrite(GREEN, HIGH);      // blinks green for good BMP values
    Serial.print("BMP280: ");       // prints values
    Serial.print("p = ");
    Serial.print(bmp_press);
    Serial.print(" Pa, temp = ");
    Serial.print(bmp_temp);
    Serial.println(" C");
    Serial.println();
    delay(150);
    digitalWrite(GREEN, LOW);
  }
  else {
    // if pressure is 0.0 or negative, blink red
    // and print 0.0 to keep formatting
    digitalWrite(RED, HIGH);
    Serial.print("BMP280: ");
    Serial.print("p = ");
    Serial.print(0.0);
    Serial.print(" Pa, temp = ");
    Serial.print(0.0);
    Serial.println(" C");
    Serial.println();
    delay(150);
    digitalWrite(RED, LOW);
  }
}

