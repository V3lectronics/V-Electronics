/*
  433 MHz RF receiver and wifi transmitter demonstration with AM2320 and HB100 sensors
  code by V Electronics JGW
  https://www.youtube.com/@V_Electronics
*/

#include <RH_ASK.h>
#include <SPI.h>
#include "WiFiS3.h"

char ssid[] = "name";        // your network SSID (name)
char pass[] = "password";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key index number (needed only for WEP)
int status = WL_IDLE_STATUS;
char server[] = "your-server-ip-or-domain";    // numeric IP can be used instead of IP to reduce sketch size
String queryString = "?data=00000.0000.00";    // initial query string

//init wifi client object
WiFiClient client;

//init amplitude shift keying object
RH_ASK rf_driver;

//HELPER FUNCTIONS
void read_response() {  
  uint32_t received_data_num = 0;
  while (client.available()) {
    /* actual data reception */
    char c = client.read();
    /* print data to serial port */
    Serial.print(c);
    /* wrap data to 80 columns*/
    received_data_num++;
    if(received_data_num % 80 == 0) { 
      Serial.println();
    }
  }  
}

void printWifiStatus() {
/* -------------------------------------------------------------------------- */  
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

//MAIN FUNCTIONS
void setup()
{
  Serial.println("BEGIN SETUP");

  //RADIO SETUP
  rf_driver.init();

  // Setup Serial Monitor
  Serial.begin(9600);
  // wait for serial port to connect. Needed for native USB port only
  // while (!Serial) {
  //   ;
  // }
  
  //WIFI SETUP
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }
  
  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }
  
  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
     
    delay(10000);
  }
  
  printWifiStatus();
 

}

void loop()
{
  // Set buffer to size of expected message
  uint8_t buf[13];
  uint8_t buflen = sizeof(buf);
  // Check if received packet is correct size
  if (rf_driver.recv(buf, &buflen))
  {
    // Message received with valid checksum
    Serial.print("Message Received: ");
    Serial.println((char*)buf);
    queryString = "?data=" + String((char*)buf);

    //Send the message over wifi as http POST request
    Serial.println("\nStarting connection to server...");
    // if you get a connection, report back via serial:
    if (client.connect(server, 80)) {
      Serial.println("connected to server");
      // Make a HTTP request:
      // client.println("GET http://example.eu/xyz/ HTTP/1.1");
      client.println("GET /iot/" + queryString + " HTTP/1.1");
      client.println("Host: example.eu");
      client.println("Connection: close");
      client.println();  // end HTTP header

      while (client.connected()) {
        if (client.available()) {
          // read an incoming byte from the server and print it to serial monitor:
          char c = client.read();
          Serial.print(c);
        }
      }

    }

    

    // read_response();

    // if the server's disconnected, stop the client:
    if (!client.connected()) {
      Serial.println();
      Serial.println("disconnecting from server.");
      client.stop();
    }
  }
}
