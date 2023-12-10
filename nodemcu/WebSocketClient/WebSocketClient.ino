#include <Arduino.h>
#include <dht11.h>



#include <ESP8266WiFi.h>
#include <Arduino_JSON.h>
#include <WebSocketsClient.h>

// #include <Hash.h>

WebSocketsClient webSocket;

#define DHT11PIN A0
dht11 DHT11;

const char *ssid = "Software_Eng";
const char *pass = "@Eng0122549";


#define SERVER "192.168.1.2"
#define PORT 3000
#define URL "/"



int lastHumidityValue = 0;
unsigned long lastSendTime = 0;  // Variable to track the last time data was sent
const int sendInterval = 1000;    // Send data every 5000 milliseconds (5 seconds)


void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
    JSONVar data; 
    const char *pumpStatus;
    int humidityValue;
    String responseStr;
	switch(type) {
		case WStype_DISCONNECTED:
			// Serial.printf("[WSc] Disconnected!\n");
			break;
		case WStype_CONNECTED: {
			// Serial.printf("[WSc] Connected to url: %s\n", payload);

			// send message to server when Connected
			// webSocket.sendTXT("Connected");
		}
			break;
		case WStype_TEXT:
        data = JSON.parse((char *)payload);
        if (JSON.typeof(data) == "undefined") {
                // Serial.println("Error: Parsing input failed!");
                return;
            }
        if (!data.hasOwnProperty("pump")) {
            //  Serial.println("Error: Payload doesn't contain the 'pump' property");
                return;
            }

            Serial.print(data);
            pumpStatus = (const char *)data["pump"];

            // Print the pump status
            Serial.print("Pump status: ");
            Serial.println(pumpStatus);

            if (strcmp(pumpStatus, "on") == 0) {
                digitalWrite(D2, HIGH);  // Turn the pin on
            } else {
                digitalWrite(D2, LOW);   // Turn the pin off
            }

            // send message to server
            // webSocket.sendTXT("message here");
			// send message to server
			// webSocket.sendTXT("message here");
			break;
		case WStype_BIN:
			// Serial.printf("[WSc] get binary length: %u\n", length);
			hexdump(payload, length);

			// send data to server
			// webSocket.sendBIN(payload, length);
			break;
    }

}

void setup() {
  pinMode(D2,OUTPUT);
  pinMode(A0,INPUT);
  pinMode(D6,INPUT);
	// Serial.begin(921600);
	Serial.begin(115200);
  
  
  WiFi.begin(ssid,pass);

  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }

	//Serial.setDebugOutput(true);
	// Serial.setDebugOutput(true);

	// Serial.println(WiFi.SSID());
	// Serial.println(WiFi.localIP());
	// Serial.println();

  webSocket.begin(SERVER,PORT,URL);




	// event handler
	webSocket.onEvent(webSocketEvent);

	// try ever 5000 again if connection has failed
	webSocket.setReconnectInterval(1000);
  
  // start heartbeat (optional)
  // ping server every 15000 ms
  // expect pong from server within 3000 ms
  // consider connection disconnected if pong is not received 2 times
  webSocket.enableHeartbeat(15000, 3000, 2);

}

void loop() {
	webSocket.loop();

//  int chk = DHT11.read(DHT11PIN);

//  Serial.println(chk);

//  Serial.print("Humidity (%): ");
//  Serial.println((float)DHT11.humidity, 2);

//   Serial.print("Temperature  (C): ");
//   Serial.println((float)DHT11.temperature, 2);
//   delay(2000);

  unsigned long currentTime = millis();
    if (currentTime - lastSendTime >= sendInterval) {
        lastSendTime = currentTime;

        // Add your periodic data sending logic here
        JSONVar data;
        int humidityValue = analogRead(A0);
        humidityValue = 100 - (humidityValue / 400 ) * 100;
        if (humidityValue < 0)  humidityValue = 0;
        Serial.println(humidityValue);
        int temperatureValue = digitalRead(D6);
        // Serial.print("Humidity Value: ");
        // Serial.println(humidityValue);
        data["humidity"] = humidityValue;
        data["temperature"] = 24;
        String responseStr = JSON.stringify(data);
        webSocket.sendTXT(responseStr);
    }
}
