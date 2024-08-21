/*
  433 MHz RF module receiver demonstration with AM2320 sensor
  code by V Electronics JGW
  https://www.youtube.com/@V_Electronics
*/

#include <RH_ASK.h>
#include <SPI.h> 

// amplitude shift keying object
RH_ASK rf_driver;

void setup()
{
    // Initialize ASK Object
    rf_driver.init();
    // Setup Serial Monitor
    Serial.begin(9600);
    Serial.println("START");

}

void loop()
{
    // Serial.println("START");
    // Set buffer to size of expected message
    uint8_t buf[13];
    uint8_t buflen = sizeof(buf);
    // Check if received packet is correct size
    if (rf_driver.recv(buf, &buflen))
    {
      
      // Message received with valid checksum
      Serial.print("Message Received: ");
      Serial.println((char*)buf);         
    }
}
