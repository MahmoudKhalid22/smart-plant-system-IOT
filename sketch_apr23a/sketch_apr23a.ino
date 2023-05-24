/*
 * WebSocketClient.ino
 *
 *  Created on: 24.05.2015
 */

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsClient.h>
#include <Arduino_JSON.h>
#include <Hash.h>

const char *HOST = "192.168.1.10";
const int PORT = 3000;
const char *WIFI_SSID = "Wee4";
const char *WIFI_PASS = "kb1856382";
  // pinMode(D0, OUTPUT);

char message[50];
int Temp = 20;
int Humidity = 10;
ESP8266WiFiMulti WiFiMulti;
WebSocketsClient webSocket;

bool pump = false;

void handleTextOrBin(uint8_t *payload, size_t length) {
  // Common logic for handling text and binary payloads

  // Parse the received JSON string
  JSONVar newState = JSON.parse((char *)payload);

  // Check if parsing succeeded
  if (JSON.typeof(newState) == "undefined") {
    Serial.println("Error: Parsing input failed!");
    return;
  }

  // Check if the "pump" property exists
  if (!newState.hasOwnProperty("pump")) {
    Serial.println("Error: Payload doesn't contain the 'pump' property");
    return;
  }

  // Get the value of the "pump" property
  bool newPump = (bool)newState["pump"];

  // Update the pump status
  pump = newPump;

  // Print the updated pump status
  Serial.print("Pump status: ");
  Serial.println(pump);
}

void webSocketEvent(WStype_t type, uint8_t *payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.println("[WSc] Disconnected!");
      break;
    case WStype_CONNECTED:
      Serial.printf("[WSc] Connected to url: %s\n", payload);

      // send message to server when Connected
      sprintf(message, "Temp: %d Hum: %d", Temp, Humidity);
      webSocket.sendTXT(message);
      break;
    case WStype_TEXT:
      Serial.printf("[WSc] get text: %s\n", payload);

      // Handle the text payload
      handleTextOrBin(payload, length);
      digitalWrite(16,pump);
      break;
    case WStype_BIN:
      Serial.printf("[WSc] get binary length: %u\n", length);

      // Handle the binary payload
      handleTextOrBin(payload, length);
      break;
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(16, OUTPUT);
  Serial.setDebugOutput(true);

  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println(pump);

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] BOOT WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFiMulti.addAP(WIFI_SSID, WIFI_PASS);

  // WiFi.disconnect();
  while (WiFiMulti.run() != WL_CONNECTED) {
    delay(100);
  }

  // server address, port, and URL
  webSocket.begin(HOST, PORT, "/");

  // event handler
  webSocket.onEvent(webSocketEvent);

  // use HTTP Basic Authorization (optional), remove if not needed
  // webSocket.setAuthorization("user", "Password");

  // try reconnecting every 5000 ms if the connection fails
  webSocket.setReconnectInterval(5000);
  
  // start heartbeat (optional)
  // ping server every 15000 ms
  // expect pong from server within 3000 ms
  // consider connection disconnected if pong is not received 2 times
  webSocket.enableHeartbeat(15000, 3000, 2);
}

void loop() {
  webSocket.loop();
  // digitalWrite(16, pump == 1 ? HIGH : LOW);
  // digitalWrite(D0, HIGH);  // turn the LED on (HIGH is the voltage level)
  // delay(1000);                      // wait for a second
  // digitalWrite(D0, LOW);   // turn the LED off by making the voltage LOW
  // delay(1000); 
}