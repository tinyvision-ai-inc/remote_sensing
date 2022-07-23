#include <PubSubClient.h>
#include "WiFi.h"
#include "certs.h"
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include "time.h"

// Wifi credentials
const char *WIFI_SSID = "VIJ-2";
const char *WIFI_PASSWORD = "9212892128";
String hostname = "TinyESP32";

#define DEVICE_NAME "Tiny-ESP32"
#define AWS_IOT_ENDPOINT "a13zm95qeyv72n-ats.iot.us-west-2.amazonaws.com"
#define AWS_IOT_TOPIC_SHADOW "$aws/things/" DEVICE_NAME "/shadow/update"
#define AWS_IOT_TOPIC_PUB DEVICE_NAME "/pub"
#define AWS_IOT_TOPIC_SUB DEVICE_NAME "/sub" //sub
#define AWS_IOT_TOPIC_LOCK DEVICE_NAME "/lock"
#define AWS_MAX_RECONNECT_TRIES 50
  
#define LED_PIN 13 

char testArray[76800];

uint32_t timer = millis();

const char* ntpServer = "pool.ntp.org";

WiFiClientSecure net = WiFiClientSecure();
PubSubClient client(net);

bool lockEngage = true;

String masterID = "A34B2B13";

void connectToWiFi()
{
  WiFi.mode(WIFI_STA);
  WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
  WiFi.setHostname(hostname.c_str());
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  int retries = 0;
  while (WiFi.status() != WL_CONNECTED && retries < 15){
    digitalWrite(LED_PIN, HIGH);
    delay(500);
    Serial.print(".");
    digitalWrite(LED_PIN, LOW);
    retries++;
  }

  // DeepSleep
  if(WiFi.status() != WL_CONNECTED){
    esp_sleep_enable_timer_wakeup(1 * 60L * 1000000L);
    esp_deep_sleep_start();
  }
  else {
    Serial.println("Connected!");
    Serial.println(WiFi.localIP());
  }
}

void connectToAWS()
{
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);

  client.setServer(AWS_IOT_ENDPOINT, 8883);
  client.setCallback(messageReceived);

  int retries = 0;
  Serial.println("Connecting to AWS IOT");

  while (!client.connect(DEVICE_NAME) && retries < AWS_MAX_RECONNECT_TRIES) {
    Serial.print(".");
    digitalWrite(LED_PIN, HIGH);
    delay(100);
    digitalWrite(LED_PIN, LOW);
    retries++;
  }
  if(!client.connected()){
    Serial.println(" Timeout!");
    return;
  }
  Serial.println("Connected to AWS!");
  //client.subscribe(AWS_IOT_TOPIC_SUB);
  client.subscribe(AWS_IOT_TOPIC_LOCK);
}

unsigned long getTime() {
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    //Serial.println("Failed to obtain time");
    return(0);
  }
  time(&now);
  return now;
}

void sendJsonToAWS()
{
  StaticJsonDocument<256> jsonDoc;
  JsonObject stateObj = jsonDoc.createNestedObject("state");
  JsonObject reportedObj = stateObj.createNestedObject("reported");
  long randNumber;
  
  reportedObj["temperature"] = random(40, 100);
  reportedObj["LAT"] = 20;
  reportedObj["LONG"] = 20;
  reportedObj["BatteryLevel"] = "79%";
  reportedObj["wifi_strength"] = WiFi.RSSI();
  reportedObj["LockStatus"] = boolToString(lockEngage);
  reportedObj["timestamp"] = getTime();
  
  //JsonObject locationObj = reportedObj.createNestedObject("location");
  //locationObj["name"] = "Garden";

  //Serial.println("Publishing message to AWS...");
  //serializeJson(doc, Serial);
  char jsonBuffer[512];
  serializeJson(jsonDoc, jsonBuffer);
  //Serial.println(jsonBuffer);
  client.publish(AWS_IOT_TOPIC_PUB, jsonBuffer);
  client.publish(AWS_IOT_TOPIC_SHADOW, jsonBuffer); //
  Serial.println("Sent Data");
}

void messageReceived(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Received [");
  Serial.print(topic);
  Serial.print("]: ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

String boolToString(bool val) {
  String converted;
  if(val){
    converted = "True";
    digitalWrite(LED_PIN, HIGH);
  }
  else {
    converted = "False";
    digitalWrite(LED_PIN, LOW);
  }
  return converted;
}

void arrayFiller() {
  for(int i=0; i<76800; i++) {
    testArray[i] = '1';
  }
}

void sendCheck() {
  if (millis() - timer > 2000) {
    timer = millis(); // reset the timer
    sendJsonToAWS();
      }
}

void setup() {
  while (!Serial);
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  connectToWiFi();
  connectToAWS();
  boolean res = client.setBufferSize(100000); if (res) Serial.println("Buffer resized."); else Serial.println("Buffer resizing failed");
  client.setKeepAlive(200);;
  client.setSocketTimeout(200);
  client.setCallback(messageReceived);
  configTime(0, 0, ntpServer);
  arrayFiller();
  randomSeed(analogRead(A7));
}

void loop() {
  if (!client.connected()) {
    connectToAWS();
  }
  client.loop();
  sendCheck();
}
