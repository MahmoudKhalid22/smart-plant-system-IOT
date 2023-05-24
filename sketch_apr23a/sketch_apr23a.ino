#include <WebSocketsClient.h>
#include <WiFi.h>

const char* ssid = "Wee4";
const char* password = "kb1856382";
const char* websockets_server_host = "localhost";
const int websockets_server_port = 3000;

WebSocketsClient webSocket;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
   while (WiFi.status() != WL_CONNECTED) {
     delay(1000);
     Serial.println("Connecting to WiFi...");
   }
  Serial.println("Connected to WiFi");

  webSocket.begin(websockets_server_host, websockets_server_port);
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  webSocket.loop();
}

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.printf("[WSc] Disconnected!\n");
      break;
    case WStype_CONNECTED:
      Serial.printf("[WSc] Connected to url: %s\n", payload);
      break;
    case WStype_TEXT:
      Serial.printf("[WSc] Received text: %s\n", payload);
      break;
    case WStype_BIN:
      Serial.printf("[WSc] Received binary data of length %u\n", length);
      break;
    default:
      break;
  }
}
