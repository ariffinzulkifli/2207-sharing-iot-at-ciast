#include <ESP8266WiFi.h>
#include <MQTT.h>

const char ssid[] = "";
const char password[] = "";
const char mqttPublishTopic[] = "";
const char mqttSubscribeTopic[] = "";

WiFiClient net;
MQTTClient mqtt;

unsigned long lastMillis = 0;

void connectToWiFi() {
  Serial.print("Connecting to Wi-Fi '" + String(ssid) + "' ...");

  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  
  Serial.println(" connected!");
}

void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
}

void connectToFavoriotMQTT(){
  Serial.print("Connecting to Favoriot MQTT ...");
  
  mqtt.begin("broker.hivemq.com", net);
  mqtt.onMessage(messageReceived);

  String uniqueString = String(ssid) + String(random(1, 100)) + String(random(100, 1000));
  char uniqueClientID[uniqueString.length() + 1];
  
  uniqueString.toCharArray(uniqueClientID, uniqueString.length() + 1);
  
  while (!mqtt.connect(uniqueClientID)) {
    Serial.print(".");
    delay(500);
  }

  Serial.println(" connected!");

  mqtt.subscribe(String(mqttSubscribeTopic) + "/v2/streams/status");
}

void setup() {
  Serial.begin(115200);
  Serial.println();

  // STEP 1 - Connecting to Wi-Fi router
  connectToWiFi();

  // STEP 2 - Connecting to MQTT broker
  connectToFavoriotMQTT();
}

void loop() {
  mqtt.loop();
  delay(10);  // <- fixes some issues with WiFi stability

  if (WiFi.status() != WL_CONNECTED) {
    connectToWiFi();
  }

  if (!mqtt.connected()) {
    connectToFavoriotMQTT();
  }

  // STEP 2 - Data Acquisition
  int randomOne = random(1, 98);
  int randomTwo = random(99, 999);

  // STEP 3 - Publish a message with given interval.
  if (millis() - lastMillis > 10000) {
    lastMillis = millis();

    String mqttPayload = "{";
    mqttPayload += "\"rone\":" + String(random(1, 10)) + ",";
    mqttPayload += "\"rtwo\":" + String(random(11, 99));
    mqttPayload += "}";
    
    mqtt.publish(String(mqttPublishTopic), mqttPayload);
  }
}
