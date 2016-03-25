/**************************************************************
 * Blynk is a platform with iOS and Android apps to control
 * Arduino, Raspberry Pi and the likes over the Internet.
 * You can easily build graphic interfaces for all your
 * projects by simply dragging and dropping widgets.
 *
 *   Downloads, docs, tutorials: http://www.blynk.cc
 *   Blynk community:            http://community.blynk.cc
 *   Social networks:            http://www.fb.com/blynkapp
 *                               http://twitter.com/blynk_app
 *
 * This example code is in public domain.
 *
 **************************************************************
 * Project setup in the Blynk app:
 *   Gauge widget on V4
 *
 **************************************************************/

#include <ESP8266WiFi.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
const char auth[] = "65d486962570457dac8686f5b9bb9a85";

// Network settings
const char ssid[] = "WiCube";
const char pass[] = "lebowski8";

// Blynk cloud server
const char* host = "blynk-cloud.com";
unsigned int port = 8080;

WiFiClient client;

void setup()
{
  Serial.begin(9600);
  delay(10);
  Serial.println();
  Serial.println();
  
  // Start the WiFi connection
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected");
}

void loop() {
  Serial.print("Connecting to ");
  Serial.print(host);
  Serial.print(":");
  Serial.print(port);
  Serial.print("... ");
  if (client.connect(host, port)) {
    Serial.println("OK");

    char pin[] = "V2";
    int value  = 123;
    
    String putData = String("[\"") + value + "\"]";
    String putReq = String("PUT /") + auth + "/pin/" + pin + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Content-Type: application/json\r\n" +
                 "Content-Length: " + putData.length() + "\r\n" +
                 "Connection: close\r\n" +
                 "\r\n" +
                 putData;

    Serial.println("Sending PUT request");
    client.print(putReq);
    //Serial.println(putReq);

    Serial.println("Waiting response");
    int timeout = millis() + 5000;
    while (client.available() == 0) {
      if (timeout - millis() < 0) {
        Serial.println(">>> Client Timeout !");
        client.stop();
        return;
      }
    }

    Serial.println("Reading response");
    int contentLength = 0;
    while (client.available()) {
      String line = client.readStringUntil('\n');
      line.trim();
      if (line.startsWith("Content-Length:")) {
        contentLength = line.substring(line.lastIndexOf(':')+1).toInt();
      } else if (line.length() == 0) {
        break;
      }
    }

    if (contentLength != 0) {
      Serial.println("WARNING: Response should be empty!");
    }
    client.stop();
    Serial.println("Done.");
    
    while(true) { delay(0); }
  } else {
    Serial.println("failed");
    delay(5000);
  }
}

