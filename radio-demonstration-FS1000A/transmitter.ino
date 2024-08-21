/*
  433 MHz RF module transmitter demonstration with AM2320 sensor
  code by V Electronics JGW
  https://www.youtube.com/@V_Electronics
*/

#include <RH_ASK.h>
#include <SPI.h> 
#include "Adafruit_Sensor.h"
#include "Adafruit_AM2320.h"
#include <string.h>

// amplitude shift keying object
RH_ASK rf_driver;

// temp and hum sensor object
Adafruit_AM2320 am2320 = Adafruit_AM2320();

char DEVICEID[] = "001";

void setup()
{
    Serial.begin(9600);
    while (!Serial) {
      delay(10); // wait until serial port opens
    }

    // Initialize Objects
    rf_driver.init();
    am2320.begin();

    // Initialize builtin LED
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.println("START");
}

void loop()
{
    digitalWrite(LED_BUILTIN, HIGH);
    String readings = String(DEVICEID) + String(am2320.readTemperature()) + String(am2320.readHumidity());

    const char *msg = readings.c_str(); //c.str() is used to convert from class String to const char*
    Serial.println("message: ");
    Serial.println(msg);
    Serial.println("size: ");
    Serial.println(strlen(msg));

    rf_driver.send((uint8_t *)msg, strlen(msg));
    rf_driver.waitPacketSent();

    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);

    // debug statements
    // Serial.println(msg);
    // Serial.println("sent packet");
    // Serial.print("Temp:"); Serial.println(am2320.readTemperature());
    // Serial.print("Hum:"); Serial.println(am2320.readHumidity());
}
