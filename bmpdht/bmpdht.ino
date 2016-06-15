   /*  Prints values of two DHT22 and one BMP280 sensors to Serial output
 *  and redirects values to data.sparkfun.com for public logging.
 *  Measures humidity, temperature (twice) and air pressure.
 *  Setup is done with I2C interface, 10k pullups are placed on SCK and SDI lines.
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
 *  Testing with Leonardo Eth and Arduino IDE 1.6.9 running on Mac OS X 10.11 (Ethernet
 *  connection not working)
 *  
 *  Last edit: June 07, 2016
 *  
 *  Andreas Hemmetter, andreas.hemmetter@gmail.com
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
#define RED 5                         // pins for LEDs
#define YELLOW 6
#define GREEN 7

// SparkFun
byte mac[] = {0x90, 0xA2, 0xDA, 0x10, 0x58, 0x88};
IPAddress server(54,86,132,254);      // numeric IP for data.sparkfun.com
IPAddress ip(141,76,111,111);
EthernetClient client;

// --- Variables --- //
int chk;                              // DHT status (0 or 1)
char COLOR;
int number;
const String publicKey = "AJaDoa9gW1SoK2D6E6QR";
const String privateKey = "rzmXDmlW90TEaVoy8yA1";
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
  Serial.begin(9600);                 // begin serial connection with 9600 baud
  delay(500);                         // delay to get connection and sensors ready
  wdt_enable(WDTO_8S);                // start watchdog timeout (4 sec might be too short)
  blink(RED, 3);                      // blinks 3 times to show board restarted
  bmp.begin();                        // starts BMP sensor
  dht1.begin();                       // starts DHT sensors
  dht2.begin();
  chk = Ethernet.begin(mac);          // starts Ethernet connection
  Serial.println(chk);                // prints Ethernet status, 0: failed, 1: ok
}

// --- Loop --- //
void loop() {
  wdt_reset();                        // resets watchdog
  delay(1800);                        // delay for sensors to be read. In total at least 2 sec!
  readdht();                          // reads DHT values
  readbmp();                          // reads BMP values
  postData();                         // posts data to data.sparkfun.com
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
  fieldData[4] = bmp.readTemperature(); // reads BMP sensor
  fieldData[5] = bmp.readPressure();
  Serial.print("BMP280: ");           // prints values
  Serial.print("p = ");
  Serial.print(fieldData[5]);         // writes value to array element
  Serial.print(" Pa, temp = ");
  Serial.print(fieldData[4]);
  Serial.println(" C");
  Serial.println();
  blink(GREEN, 1);                    // LED check
}

void readdht() {
  fieldData[0] = dht1.readTemperature();
  fieldData[1] = dht1.readHumidity();   // reads DHT sensor
  fieldData[2] = dht2.readTemperature();
  fieldData[3] = dht2.readHumidity();   // reads DHT sensor
  Serial.print("DHT22_2: ");          // prints values (hum and temp)
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

void postData()
{
  // Make a TCP connection to remote host
  client.connect(server, 80);         // starts client connection to server
  delay(1000);
  if (client.connect(server, 80))
  {
    // Post the data! Request should look a little something like:
    // GET /input/publicKey?private_key=privateKey&light=1024&switch=0&name=Jim HTTP/1.1\n
    // Host: data.sparkfun.com\n
    // Connection: close\n
    // \n
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
    Serial.println("Connection failed");
  } 

  // Check for a response from the server, and route it
  // out the serial port.
  
  while (client.connected())
  {
    if ( client.available() )
    {
      char c = client.read();
      Serial.print(c);
    }      
  }
  
  Serial.println();
  client.stop();
}
