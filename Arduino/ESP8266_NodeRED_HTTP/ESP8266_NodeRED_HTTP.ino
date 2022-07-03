#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char ssid[] = "";
const char password[] = "";
const char noderedServer[] = "192.168.x.x:1880";
const char noderedURL[] = "";

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

void setup() {
  Serial.begin(115200);

  // STEP 1 - Connect to Wi-Fi router/hotspot
  connectToWiFi();

}

void loop() {

  if (WiFi.status() != WL_CONNECTED) {
    connectToWiFi();
  }
  
  // STEP 2 - Data Acquisition
  int randomOne = random(1, 98);
  int randomTwo = random(99, 999); 

  if(millis() - lastMillis > 15000){
    lastMillis = millis();

    Serial.println("Generated Data:");
    Serial.println("RandomOne: " + String(randomOne) + ", RandomTwo: " + String(randomTwo));

    // STEP 3 - Generate data in URL query string format
    String httpRequest = "rone=" + String(randomOne) + "&rtwo=" + String(randomTwo);
    
    // STEP 4 - HTTP API Request to Node-RED with URL /data
    WiFiClient client;
    HTTPClient http;
  
    http.begin(client, "http://" + String(noderedServer) + "/" + String(noderedURL) + "?" + httpRequest);
    
    int httpCode = http.GET();
    
    if(httpCode > 0){
      Serial.print("HTTP Request: ");
      
      httpCode == 200 ? Serial.print("Success, ") : Serial.print("Error, ");
      Serial.println(http.getString());
    }
    else{
      Serial.println("HTTP Request Connection Error!");
    }
  
    http.end();
  }
}
