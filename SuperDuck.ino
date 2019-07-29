#include <WiFi.h>
#include <WebServer.h>
#include <pthread.h>

// Configure your WiFi here:
const char* ssid = "";  // Enter SSID here
const char* password = "";  //Enter Password here

// declare an object of WebServer library
WebServer server(80);

// Variable to store the HTTP request
String header;

// Assign variables
const byte ledPins[] = {4, 5, 21, 22, 13, 27, 33};
unsigned long timer = 200; // the higher the number, the slower the timing.

// Set up a thread
pthread_t threads[1];

bool busy = false;

void setup() {
  Serial.begin(115200);

  unsigned long startmillis = millis();
  while ((millis() - startmillis) < 100) {
    yield();
  }
  
  //initialize ledPins array as outputs
  for (int i = 0; i< (sizeof(ledPins) / sizeof(ledPins[0])); i++) {
    pinMode(ledPins[i], OUTPUT);
    pinMode(BUILTIN_LED, OUTPUT); 
  }

  //we start by connecting to a WiFi network
  Serial.print("Connecting to "); Serial.println(ssid);

  //connect WiFi to local network
  WiFi.begin(ssid, password);

  //check WiFi status
  while (WiFi.status() != WL_CONNECTED) {

  unsigned long startmillis = millis();
  while ((millis() - startmillis) < 200) {
    yield();
  }

  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.print("Got IP: "); Serial.println(WiFi.localIP());
  yield();

  server.on("/", handle_OnConnect);
  server.on("/alert", handle_alert);
  server.onNotFound(handle_NotFound);
        
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}

void handle_OnConnect() {
  server.send(200); 
}

void handle_alert() {
  if (not busy) {
    pthread_create(&threads[0], NULL, alert, (void *)0);
  }
}

void *alert(void *threadid) {
  Serial.println("Alerting");
  busy = true;
  server.send(200); 
  int repeat = 0;
  unsigned long startmillis;
  
  while (repeat < 20) {
    for (int i = 0; i< (sizeof(ledPins) / sizeof(ledPins[0])); i++) {
      digitalWrite(ledPins[i], HIGH);
      digitalWrite(BUILTIN_LED, HIGH);
      yield();
    }
    
    startmillis = millis();
    while ((millis() - startmillis) < timer) {
      yield();
    }
    
    for (int i = 0; i< (sizeof(ledPins) / sizeof(ledPins[0])); i++) {
      digitalWrite(ledPins[i], LOW);
      digitalWrite(BUILTIN_LED, LOW);
      yield();
    }

    startmillis = millis();
    while ((millis() - startmillis) < timer) {
      yield();
    }

    repeat++;
    busy = false;
  }
}

void handle_NotFound(){
  server.send(404);
}
