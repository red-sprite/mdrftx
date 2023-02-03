// Required for DS18B20
#include <OneWire.h>
#include <DallasTemperature.h>
// Required for RadioHead
// Include RadioHead Amplitude Shift Keying Library
#include <RH_ASK.h>
// Include dependant SPI Library 
#include <SPI.h> 

// DS18B20 information
// Data wire for temperature sensor is plugged into digital pin 16 on the Arduino
#define ONE_WIRE_BUS 16
// setup a onwWire instance to communicate with oneWire devices
OneWire oneWire(ONE_WIRE_BUS);
// pass oneWire reference to Dallas Temperature library
DallasTemperature ds18b20(&oneWire);

// RadioHead information
// Create Amplitude Shift Keying Object
RH_ASK rf_driver;

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  // Set up temperature sensor
  ds18b20.begin();
  // Initialize ASK Object
  rf_driver.init();
  // set up serial port for diagnostics
  Serial.begin(115200);
}

void loop() {
  float temp;
  // flash LED
  //Serial.println("Start of loop");
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(100);                      // wait for a second
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW

  // read temperature sensor and print
  // send the command to get temperatures
  ds18b20.requestTemperatures();
  // print the temperature in Celsius
  Serial.printf("%8d", millis());
  Serial.print(" Temperature: ");
  temp = ds18b20.getTempCByIndex(0);
  Serial.print(temp);
  Serial.write(0xC2); // show degress character
  Serial.write(0xB0);
  Serial.println("C");

  // send message via RF 433MHz
  //const char *msg = "Hello World";
  char buf[40];
  int bufSize;
  sprintf(buf, "T=%3.3f", temp);
  bufSize = strlen(buf);
  rf_driver.send((uint8_t *)buf, bufSize);
  rf_driver.waitPacketSent();
  Serial.printf("Sent data via RF 433 MHz, %d bytes >%s<\n", bufSize, buf);
  
  delay(1000);
  //Serial.println("After delay");
}
