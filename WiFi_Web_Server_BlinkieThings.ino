/*
 WiFi Web Server LED Blink
 A simple web server that lets you blink an LED via the web.
 This sketch will print the IP address of your WiFi Shield (once connected)
 to the Serial monitor. From there, you can open that address in a web browser
 to turn on and off the LED on pin 5.
 If the IP address of your shield is yourAddress:
 http://<yourAddress>/alert turns the LED on
 This example is written for a network using WPA encryption. For
 WEP or WPA, change the Wifi.begin() call accordingly.
 Circuit:
 * WiFi shield attached
 * LED attached to pin 5
 */

#include <WiFi.h>

const char* ssid     = "<Enter SSID here>";
const char* password = "<Enter WPA here>";
int ext_led = 5;
int timer = 500;
int i;

WiFiServer server(80);

void setup()
{
    Serial.begin(115200);
    pinMode(5, OUTPUT);
    pinMode(BUILTIN_LED, OUTPUT); //set the internal LED pin mode
                 
    delay(10);

    // We start by connecting to a WiFi network

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    
    server.begin();          
  
}

int value = 0;

void loop(){
 WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");          // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("Click <a href=\"/alert\">here</a> to test LEDs<br>");
            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /alert":
        for (i = 0; i<=5; i++) {
          if (currentLine.endsWith("GET /alert")) 
          {
            // turn the pins on:
            digitalWrite(ext_led, HIGH);
            digitalWrite(BUILTIN_LED, HIGH);
            delay(timer);
            // turn the pins off:
            digitalWrite(ext_led, LOW);
            digitalWrite(BUILTIN_LED, LOW); 
            delay(timer);
          }

        }
        
      
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
