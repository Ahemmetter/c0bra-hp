<<<<<<< HEAD
/*  Prints values of two DHT22 and one BMP280 sensors to Serial output
=======
 /*  Prints values of two DHT22 and one BMP280 sensors to Serial output
>>>>>>> d5d2c06e0f16e3673508c13ca6ae020f8b1b4fc1
 *  and redirects values to data.sparkfun.com for public logging.
 *  Measures humidity, temperature (twice) and air pressure.
 *  Setup is done with I2C interface, 10k pullups are placed on
 *  SCK and SDI lines.
 *
 *  LED patterns:
 *  --------------------------------------------------------
 *  YELLOW on             : power, Serial connection
 *  GREEN blinks 2x       : 2 measurements (#1 DHT, #2 BMP)
 *  GREEN blink, then RED : BMP sensor not returning values
 *  RED blink, then GREEN : DHT not returning values
 *  RED blinks 2x         : either sensor not connected
 *  RED blinks 3x         : reboot
 *
 *  Watchdog counts till 8 seconds and reboots Arduino board,
 *  unless it has been
 *  reset. This catches infinite loops and general problems.
 *
 *  Pullups seem to be not necessary in either case. LEDs are connected
 *  to a 390 Ohm resistor each.
 *
 *  Tested with Arduino Uno SMD R3 board and Arduino IDE 1.6.8 running
 *  on Ubuntu 14.04 (Ethernet working)
 *  Testing with Leonardo Eth and Arduino IDE 1.6.9 running
 *  on Mac OS X 10.11 (Ethernet connection not working)
 *
 *  Last edit: June 29, 2016
 *
 *  Andreas Hemmetter, andreas.hemmetter@gmail.com
 *  for IKTP at TU Dresden
 *  https://github.com/Ahemmetter/c0bra-hp
 */

// --- Libraries --- //
#include <DHT.h>                      // DHT library for DHT22
#include <avr/wdt.h>                  // Watchdog, interrupts and reboots
#include <Adafruit_BMP280.h>          // BMP library
#include <Adafruit_Sensor.h>          // dependent library
#include <Wire.h>                     // dependent library
#include <SPI.h>                      // library for SPI connections
#include <Ethernet.h>                 // Ethernet library

// --- Constants --- //
// DHT22
#define DHTPIN_1 12                   // pins for DHT data
#define DHTPIN_2 11

// LEDs
#define RED 10                         // pins for LEDs
#define YELLOW 9
#define GREEN 8

// SparkFun
byte mac[] = {0x90, 0xA2, 0xDA, 0x10, 0x58, 0x88};
IPAddress server(54,86,132,254);      // numeric IP for data.sparkfun.com
//IPAddress ip(10,100,60,100);
EthernetClient client;

// --- Variables --- //
int chk;                              // DHT status (0 or 1)
char COLOR;
int number;
const String publicKey = "AJaDoa9gW1SoK2D6E6QR";
const String privateKey = "***"; // left out for privacy reasons
const byte NUM_FIELDS = 6;
const String fieldNames[NUM_FIELDS] = {"dht1_t" "dht1_h", "dht2_t", "dht2_h", "bmp_t", "bmp_p"};
String fieldData[NUM_FIELDS];

// --- Initialization --- //
DHT dht1(DHTPIN_1, DHT22);            // initializes sensors
DHT dht2(DHTPIN_2, DHT22);
Adafruit_BMP280 bmp;

// --- Setup --- //
void setup() {
  wdt_disable();                      // disables watchdog for initial communication
  pinMode(YELLOW, OUTPUT);            // LED pins set as output
  digitalWrite(YELLOW, HIGH);         // starts yellow LED
  pinMode(RED, OUTPUT);            // LED pins set as output
<<<<<<< HEAD
  digitalWrite(RED, LOW);         // starts red LED
  pinMode(GREEN, OUTPUT);            // LED pins set as output
  digitalWrite(GREEN, LOW);         // starts green LED
=======
  digitalWrite(RED, LOW);         // starts yellow LED
  pinMode(GREEN, OUTPUT);            // LED pins set as output
  digitalWrite(GREEN, LOW);         // starts yellow LED
>>>>>>> d5d2c06e0f16e3673508c13ca6ae020f8b1b4fc1
  Serial.begin(9600);                 // begin serial connection with 9600 baud
  delay(500);                         // delay to get connection and sensors ready
  wdt_enable(WDTO_8S);                // start watchdog timeout
  blink(RED, 3);                      // blinks 3 times to show board restarted
  bmp.begin();                        // starts BMP sensor
  dht1.begin();                       // starts DHT sensors
  dht2.begin();
  Ethernet.begin(mac);          // starts Ethernet connection
}

// --- Loop --- //
void loop() {
  wdt_reset();                        // resets watchdog
  delay(300000);                        // delay for sensors to be read. In total at least 2 sec!
  readdht();                          // reads DHT values
  readbmp();                          // reads BMP values
  //Serial.println("done with reading");
  postData();                         // posts data to data.sparkfun.com
  //Serial.println("done with writing");
}

void blink(char COLOR, int number) {
  int k;                              // counter variable
  for (k = 1; k <= number; k = k + 1) {
    digitalWrite(COLOR, HIGH);
    delay(150);
    digitalWrite(COLOR, LOW);
    delay(150);
  }
}

void readbmp() {
<<<<<<< HEAD
    // if sensor isnt attached, it shows 0.0 or negative pressure.
    // This is used as check criterion
    fieldData[4] = bmp.readTemperature(); // reads BMP sensor
    fieldData[5] = bmp.readPressure();
  if (bmp.readPressure() > 0.0) {
        Serial.print("BMP280: ");           // prints values
        Serial.print("p = ");
        Serial.print(fieldData[5]);         // writes value to array element
        Serial.print(" Pa, temp = ");
        Serial.print(fieldData[4]);
        Serial.println(" C");
        blink(GREEN, 1);                    // LED check
    }
    else {
        blink(RED,1);
        delay(100);
    }
}

void readdht() {
  chk = dht1.read(DHTPIN_1) + dht2.read(DHTPIN_2);           // checks if connection to DHT sensor is ok
    if (chk == 2) {
        fieldData[0] = dht1.readTemperature();
        fieldData[1] = dht1.readHumidity();   // reads DHT sensor
        fieldData[2] = dht2.readTemperature();
        fieldData[3] = dht2.readHumidity();   // reads DHT sensor
        Serial.print("DHT22_1: ");          // prints values (hum and temp)
        Serial.print("hum = ");
        Serial.print(fieldData[1]);         // writes values to array elements
        Serial.print(" %, temp = ");
        Serial.print(fieldData[0]);
        Serial.println(" C");
        Serial.print("DHT22_2: ");          // prints values (hum and temp)
        Serial.print("hum = ");
        Serial.print(fieldData[3]);
        Serial.print(" %, temp = ");
        Serial.print(fieldData[2]);
        Serial.println(" C");
        blink(GREEN, 1);
        delay(100);
    }
  else {
      blink(RED, 1);
      delay(100);
    }
=======
  fieldData[4] = bmp.readTemperature(); // reads BMP sensor
  fieldData[5] = bmp.readPressure();
  Serial.print("BMP280: ");           // prints values
  Serial.print("p = ");
  Serial.print(fieldData[5]);         // writes value to array element
  Serial.print(" Pa, temp = ");
  Serial.print(fieldData[4]);
  Serial.println(" C");
  blink(GREEN, 1);                    // LED check
}

void readdht() {
  fieldData[0] = dht1.readTemperature();
  fieldData[1] = dht1.readHumidity();   // reads DHT sensor
  fieldData[2] = dht2.readTemperature();
  fieldData[3] = dht2.readHumidity();   // reads DHT sensor
  Serial.print("DHT22_1: ");          // prints values (hum and temp)
  Serial.print("hum = ");
  Serial.print(fieldData[1]);         // writes values to array elements
  Serial.print(" %, temp = ");
  Serial.print(fieldData[0]);
  Serial.println(" C");
  Serial.print("DHT22_2: ");          // prints values (hum and temp)
  Serial.print("hum = ");
  Serial.print(fieldData[3]);
  Serial.print(" %, temp = ");
  Serial.print(fieldData[2]);
  Serial.println(" C");
  blink(GREEN, 1);
  delay(100);
>>>>>>> d5d2c06e0f16e3673508c13ca6ae020f8b1b4fc1
}

void postData()
{
  //Serial.println("ok im here");
  // Make a TCP connection to remote host
  client.connect(server, 80);         // starts client connection to server
  delay(1000);
  if (client.connect(server, 80))
  {
    client.print("GET /input/");      // writes HTML request
    client.print(publicKey);
    client.print("?private_key=");
    client.print(privateKey);
    for (int i=0; i<NUM_FIELDS; i++)  // writes values
    {
      client.print("&");
      client.print(fieldNames[i]);
      client.print("=");
      client.print(fieldData[i]);
    }
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(server);
    client.println("Connection: close");
    client.println();
  }
  else
  {
<<<<<<< HEAD
    Serial.println("Connection failed");
  }
=======
    //Serial.println("Connection failed");
  } 

  // Check for a response from the server, and route it
  // out the serial port.
  
>>>>>>> d5d2c06e0f16e3673508c13ca6ae020f8b1b4fc1
  while (client.connected())
  {
    if ( client.available() )
    {
      char c = client.read();
      //Serial.print(c);
<<<<<<< HEAD
    }
=======
    }      
>>>>>>> d5d2c06e0f16e3673508c13ca6ae020f8b1b4fc1
  }
  Serial.println();
  client.stop();
}
