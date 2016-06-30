/*  Prints values of DHT22 and BMP280 sensors to Serial output.
 *  Logging is done with ThingSpeak.com, connected via Ethernet port.
 *  
 *  Measures humidity, temperature (twice) and air pressure.
 *  Setup is done with SPI interface, 10k pullups are placed on SCK, SDO and CS lines.
 *  
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
 *  ThingSpeak allows updates only every 15 seconds, so watchdog needs to be restarted
 *  several times during each loop (delay is split up).
 *  
 *  Issues: CS seems not to work and doesnt initialize BMP sensor.
 *  
 *  Pullups seem to be not necessary in either case. LEDs are connected to a
 *  390 Ohm resistor each.
 *  
 *  Tested with Arduino Uno SMD R3 board and Arduino IDE 1.6.8 running on Ubuntu 14.04
 *  Tested with Arduino Ethernet board   and Arduino IDE 1.6.8 running on Ubuntu 16.04
 *  
 *  Last edit: May 13, 2016
 *  
 *  Andreas Hemmetter, andreas.hemmetter@gmail.com
 *  https://github.com/Ahemmetter/c0bra-hp
 *  https://thingspeak.com/channels/115634
 */

#include "ThingSpeak.h"
#include <SPI.h>
#include <DHT.h>
#include <Ethernet.h>
byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0xE5, 0xBF};
IPAddress ip(10,42,0,69);
EthernetClient client;


// --- Libraries --- //
#include <avr/wdt.h>                // Watchdog, interrupts and reboots     
#include <Adafruit_BMP280.h>        // BMP library for BMP280


// --- Constants --- //
// DHT22
#define DHTPIN 7

// BMP280 (SPI)
#define BMP_SCK 13                  // Serial clock pin for BMP (10k pullup to +5V)
#define BMP_MISO 12                 // SDO, has 10k pullup
#define BMP_MOSI 11                 // SDI, no pullup
#define BMP_CS 10                    // CS, has 10k pullup

// LEDs
#define RED 5                       // pin for LEDs
#define YELLOW 3
#define GREEN 2



unsigned long myChannelNumber = 115634;
const char * myWriteAPIKey = "266SWBGL2R8WWHN8";
float dht_temp;
float dht_hum;
float bmp_temp;                     // values from sensors
float bmp_press;
int chk;                            // DHT status (0 or 1)
int k;                              // counter variable
int number;
char COLOR;


// --- Initialization --- //
DHT dht(DHTPIN, DHT22);             // initializes sensors
Adafruit_BMP280 bmp(BMP_CS, BMP_MOSI, BMP_MISO, BMP_SCK);


void setup() {
  wdt_disable();                    // disables watchdog for initial communication
  
  
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
  
  pinMode(YELLOW, OUTPUT);          // LED pins set as output
  pinMode(RED, OUTPUT);
  digitalWrite(YELLOW, HIGH);       // starts yellow LED
  Serial.begin(9600);               // begin serial connection with 9600 baud
  delay(500);                       // delay to get connection and sensors ready
  wdt_enable(WDTO_8S);              // start watchdog timeout (4 sec might be too short)
  bmp.begin();
  blink(RED, 3);
  dht.begin();                      // starts DHT sensor
  Serial.println(bmp.begin());
  
  chk = dht.read(DHTPIN);           // reads DHT status
  Serial.println(chk);
  
  if ((!bmp.begin()) || (chk != 1)) {
    // checks DHT and BMP status and shows red LED, in case one of them
    // wasnt found. Loops and resets timeout each time.
    wdt_reset();
    bmp.begin();
    Serial.println("Could not find a valid sensor, check wiring!");
    digitalWrite(RED, HIGH);
    delay(300);
    digitalWrite(RED, LOW);
    delay(1000);
    Serial.println(bmp.begin());
    
    bmp.begin();
    chk = dht.read(DHTPIN);
    Serial.println(chk);
    delay(1000);
  }
  Ethernet.begin(mac, ip);
  ThingSpeak.begin(client);
}

// --- Loop --- //
void loop() {
  wdt_reset();                      // resets watchdog
  pinMode(GREEN, OUTPUT);           // gets green LED ready
  digitalWrite(GREEN, LOW);
  delay(1600);                      // delay for sensors to be read. In total, at least 2 sec!
  
  dhtread();

  delay(5000);
  wdt_reset();

  bmpread();
  
  ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  delay(5000);
  wdt_reset();
  delay(5000);
}


void blink(char COLOR, int number) {
  for (k = 1; k <= number; k = k + 1) {
    // blinks 3x to show that Arduino rebooted
    digitalWrite(COLOR, HIGH);
    delay(100);
    digitalWrite(COLOR, LOW);
    delay(100);
  }
}

void bmpread() {
  bmp_temp = bmp.readTemperature(); // reads BMP sensor
  bmp_press = bmp.readPressure();
  Serial.println(bmp_temp);
  Serial.println(bmp_press);
  
  if (bmp_press > 0.0) {
    // if sensor isnt attached, it shows 0.0 or negative pressure.
    // This is used as check criterion
    digitalWrite(GREEN, HIGH);      // blinks green for good BMP values
    Serial.println("bmp ok");
    ThingSpeak.setField(3,bmp_temp);
    ThingSpeak.setField(4,bmp_press);
    delay(150);
    digitalWrite(GREEN, LOW);
  }
  
  else {
    // if pressure is 0.0 or negative, blink red
    // and print 0.0 to keep formatting
    digitalWrite(RED, HIGH);
    Serial.println("Error, BMP280 missing");
    delay(150);
    digitalWrite(RED, LOW);
  }
}

void dhtread() {
  chk = dht.read(DHTPIN);           // checks if connection to DHT sensor is ok (0: no connection, 1: ok)
  if (chk == 1) {
    dht_temp = dht.readTemperature();
    dht_hum = dht.readHumidity();   // reads DHT sensor
    digitalWrite(GREEN, HIGH);      // blinks once for successful DHT sensor values
    ThingSpeak.setField(1,dht_temp);
    ThingSpeak.setField(2,dht_hum);
    delay(150);
    digitalWrite(GREEN, LOW);
    delay(100);
  }
  else {
    // if no connection, blink red instead and print "nan"
    // (keeps formatting ok)
    digitalWrite(RED, HIGH);
    Serial.print("Error, DHT22 missing");
    delay(150);
    digitalWrite(RED, LOW);
    delay(100);
  }
}

