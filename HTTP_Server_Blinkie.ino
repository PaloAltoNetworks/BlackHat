/**
 * This sketchbook was built for the ESP32 to server as a HTTP Webserver. The purpose of this was built to host 
 * a web server that the Palo Alto Networks NGFW can make HTTP Post calls to and provide a visual notification 
 * when an alert triggers from NGFW.
 *
 * WARNING:
 * __________
 * This is an HTTP Webserver over tcp/80. None of the data will be encrypted. If you would like to 
 * encrypt your ES32 then please import and configure a TLS-enabled web server. A great example can be found here:
 * https://github.com/fhessel/esp32_https_server
 * __________
 * 
 */

#include <WiFi.h>
#include <WebServer.h>

// TODO: Configure your WiFi here:
const char* ssid = "Your SSID Goes Here";  // Enter SSID here
const char* password = "Your Pre-Shared Key Goes Here";  //Enter Password here

// declare an object of WebServer library
WebServer server(80);

//Variable to store the HTTP request
String header;

// Auxiliary variables to store the current output state
bool LEDstate = LOW;

// Assign variables
#define numberOfLights 2      // update this variable if you increase the GPIO pins
int ledPins[] = {4,5};
int pinState[numberOfLights];
int timer = 500;              // the higher the number, the slower the timing.
int i;

void setup() {
  Serial.begin(115200);
  delay(10);
  
  //initialize ledPins array as outputs
  for (int i = 0; i< numberOfLights; i++) {
    pinMode(ledPins[i], OUTPUT);
    pinMode(BUILTIN_LED, OUTPUT); 
    pinState[i] = LOW; 
  }

  //we start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to "); Serial.println(ssid);

  //connect WiFi to local network
  WiFi.begin(ssid, password);

  //check WiFi status
  while (WiFi.status() != WL_CONNECTED) {
  delay(timer);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.print("Got IP: "); Serial.println(WiFi.localIP());

  server.on("/", handle_OnConnect);
  server.on("/LED/on", handle_on);
  server.on("/LED/off", handle_off);
  server.on("/LED/wave", handle_wave);
  server.on("/alert", handle_alert);
  server.onNotFound(handle_NotFound);
        
  server.begin();
  Serial.println("HTTP server started");
  Serial.println("Navigate to the IP in your browser to see it working");

}

void loop() {
  
  server.handleClient();
  
  for (int i = 0; i < numberOfLights; i++) {
    if(LEDstate) {
      pinState[i] = ~pinState[i];
      digitalWrite(ledPins[i], pinState[i]);
  }
 }
}

void handle_OnConnect() {
  LEDstate = LOW;
  server.send(200, "text/html", SendHTML(LEDstate)); 
}

void handle_on() {
  for (int i = 0; i< numberOfLights; i++) {
    digitalWrite(ledPins[i], HIGH);
    digitalWrite(BUILTIN_LED, HIGH);
  }  
  Serial.println("LED Status: ON"); 
  LEDstate = HIGH;
  server.send(200, "text/html", SendHTML(LEDstate)); 
}

void handle_off() {
  for (int i = 0; i< numberOfLights; i++) {
    digitalWrite(ledPins[i], LOW);
    digitalWrite(BUILTIN_LED, LOW);
  }
  Serial.println("LED Status: OFF");
  LEDstate = LOW;
  server.send(200, "text/html", SendHTML(LEDstate)); 
}

void handle_wave() {
  int repeat = 0;
  while (repeat < 5) {
    for (int i = 0; i <= numberOfLights; i++) {
      digitalWrite(ledPins[i], HIGH);
      digitalWrite(BUILTIN_LED, HIGH);
      delay(timer);
      digitalWrite(ledPins[i], LOW);
      digitalWrite(BUILTIN_LED, LOW);
    }
    for (int i = 0; i >= numberOfLights; i--) {
      digitalWrite(ledPins[i], HIGH);
      digitalWrite(BUILTIN_LED, HIGH);
      delay(timer);
      digitalWrite(ledPins[i], LOW);
      digitalWrite(BUILTIN_LED, LOW);
    }
    repeat++;
  }
  Serial.println("Doin' the W-A-V-E!");
  server.send(200, "text/html", SendHTML(LEDstate)); 
}

void handle_alert() {
  int repeat = 0;
  while (repeat < 5) {
    for (int i = 0; i< numberOfLights; i++) {
      //turn the LEDs on:
      digitalWrite(ledPins[i], HIGH);
      digitalWrite(BUILTIN_LED, HIGH);
      delay(timer);
      //turn the LEDs off:
      digitalWrite(ledPins[i], LOW);
      digitalWrite(BUILTIN_LED, LOW);
      delay(200);
    }
    repeat++;
    }
  Serial.println("RED ALERT!");
  server.send(200, "text/html", SendHTML(LEDstate)); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Oopsie, you took a wrong turn somewhere...");
}

/// HTML Web Page
String SendHTML(uint8_t ledstatus){
  String ptr = "<!DOCTYPE html> <html>";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  ptr +="<link rel=\"icon\" href=\"data:,\">";
  // CSS to style the on/off buttons 
  // Feel free to change the background-color and font-size attributes to fit your preferences
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}";
  ptr +=".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;";
  ptr +="text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}";
  ptr +=".button2 {background-color: #555555;}";
  ptr +=".button3 {background-color: #CC0000;}";
  ptr +=".button4 {background-color: #FF8000;}";
  ptr +="</style></head>";
  // Web Page Heading
  ptr +="<body><h1>DuckDuckSOC Web Server</h1>";
  ptr +="<img src=https://i.imgur.com/qtpFo58.png title=Quack! Quack! align=top height=300 width=300>";
  
    // If the LEDstate is off, it displays the ON button
    if (ledstatus) {
      ptr +="<p>LED Status: ON</p><a href=\"/LED/off\"><button class=\"button button2\">OFF</button></a>";
      } else { 
        ptr +="<p>LED Status: OFF</p><a href=\"/LED/on\"><button class=\"button\">ON</button></a>";
    }
        
  ptr +="<p><a href=\"/LED/wave\"><button class=\"button button4\">WAVE</button></p></a>";
  ptr +="<p><a href=\"/alert\"><button class=\"button button3\">ALERT</button></p></a>";

  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
