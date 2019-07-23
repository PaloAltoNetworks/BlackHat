/*
 WiFi Web Server LED Blink
 A simple web server that lets you blink the LEDs via a web server.
 This sketch will print the IP address of your WiFi Shield (once connected)
 to the Serial monitor. From there, you can open that address in a web browser
 to turn on and off the LED on the assigned pins (pin 5 is the default)
 Replace <yourAddress> with the IP address of the ESP32:
 http://<yourAddress>/ 
 
 For the NGFW you will send calls to:
 http://<yourAddress>/alert
 turns the LED on
 
 This example is written for a network using WPA encryption. For
 WEP or WPA, change the Wifi.begin() call accordingly.
 Circuit:
 * WiFi shield attached
 * LED attached to pin 5
 */

#include <WiFi.h>

const char* ssid     = "<Your SSID Goes Here>"; // ** Enter in the SSID you want to connect to **
const char* password = "<Your Pre-Shared Key Goes Here>"; // ** Enter the password for the SSID **

// Set web server port number to 80
WiFiServer server(80);

//Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String LEDstate = "off";

// Assign variables
int ledPins[] = {5, 4};       // ** an array of pin numbers to which LEDs are attached **
int pinCount = 2;             // ** update this to the respective value from the GPIO variable input **
//const int ext_led = 5;      // update this to whatever GPIO you use for LEDs
int timer = 500;              // the higher the number, the slower the timing.
int i;

void setup() {
    Serial.begin(115200);
    delay(10);
    
    // Initialize the output variables as outputs
    // the array elements are numbered from 0 to (pinCount - 1).
    // use a for loop to initialize each pin as an output:
 for (int extPin = 0; extPin < pinCount; extPin++) {
    pinMode(ledPins[extPin], OUTPUT);
    pinMode(BUILTIN_LED, OUTPUT);  
  //pinMode(ext_led, OUTPUT);
 }
 
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
    Serial.println(WiFi.localIP()) "/ in your browser to see it working");
    
    server.begin();
}


void pinOn(int extPin) {
    // turn the pin on:
    digitalWrite(ledPins[extPin], HIGH);
    digitalWrite(BUILTIN_LED, HIGH); 
    Serial.print(extPin);
    Serial.println("LEDs are ON");

    delay(timer);
 
    readPin(extPin);
    
    Serial.println();
    Serial.println();
}


void pinOff(int extPin) {
    // turn the pin off:
    digitalWrite(ledPins[extPin], LOW);
    digitalWrite(BUILTIN_LED, LOW); 
    Serial.print(extPin);
    Serial.println("LEDs are OFF");

    delay(timer);
 
    readPin(extPin);
  
    Serial.println();
    Serial.println();
}


void wave() {
 // loop from the lowest pin to the highest:
 for (int extPin = 0; extPin < pinCount; extPin++) {
  on(extPin);
  off(extPin);
 }
}


void loop() {
 WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");          // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
           
          // Check to see if the client request was "GET /alert" for NGFW HTTP POST:
           for (i = 0; i<=5; i++) {
            if (currentLine.endsWith("GET /alert")) {
             // turn the pins on:
             pinOn(extPin);
             // turn the pins off:
             pinOff(extPin);
            }
           }
           
           
           // Check to see if the button Wave was pressed:
           for (i = 0; i<=5; i++) {
            if (currentLine.endsWith("GET /WAVE")) {
             wave(extPin);
            }
           }
            
            // turns the LEDs on and off
            if (header.indexOf("GET /LED/on") >= 0) {
              Serial.println("LEDs On");
              LEDstate = "on";
              pinOn(extPin);
            } else if (header.indexOf("GET /LED/off") >= 0) {
              Serial.println("LEDs Off");
              LEDstate = "off";
              pinOff(extPin);
            } 
            
           
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            // Web Page Heading
            client.println("<body><h1>DuckDuckSOC Web Server</h1>");
            client.println("<img src=https://i.imgur.com/qtpFo58.png title=Quack! Quack! align=top height=300 width=300>");
            
            // Display current state, and ON/OFF buttons for ext_led  
            client.println("<p>LED State: " + LEDstate + "</p>");
            // If the LEDstate is off, it displays the ON button       
            if (LEDstate=="off") {
              client.println("<p><a href=\"/LED/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/LED/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            client.print("<center><p>Wave<b>(2 Secs Aprox)</b><a href=\"WAVE\"><button>Start</button></center>");
            client.println("</body></html>");

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
      }
    }
    // close the connection:
    header = "";
    client.stop();
    Serial.println("Client Disconnected.");
    Serial.println("");
  }
}
