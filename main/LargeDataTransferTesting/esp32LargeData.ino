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

const int arrWidth = 320; //320
const int arrLength = 240; //240

char testArray[arrWidth][arrLength]; //char takes 2bytes and int is 4 bytes so have to use char

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

void testSend() {
  String msg = "1111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111\n";
  uint32_t msgLen = arrLength*arrWidth;
  msgLen = (arrLength * msg.length());
  digitalWrite(LED_PIN, HIGH);
  client.beginPublish(AWS_IOT_TOPIC_PUB, msgLen, false);
  for(int y=0; y<arrLength; y++) {
    //for(int x=0; x<arrWidth; x++) {
      //client.print(testArray[x][y]);
    //}
    client.print(msg);
    //client.print("\n");
  }
  client.endPublish();
}

void pubSubErr(int8_t MQTTErr)
{
  if (MQTTErr == MQTT_CONNECTION_TIMEOUT)
    Serial.print("Connection tiemout");
  else if (MQTTErr == MQTT_CONNECTION_LOST)
    Serial.print("Connection lost");
  else if (MQTTErr == MQTT_CONNECT_FAILED)
    Serial.print("Connect failed");
  else if (MQTTErr == MQTT_DISCONNECTED)
    Serial.print("Disconnected");
  else if (MQTTErr == MQTT_CONNECTED)
    Serial.print("Connected");
  else if (MQTTErr == MQTT_CONNECT_BAD_PROTOCOL)
    Serial.print("Connect bad protocol");
  else if (MQTTErr == MQTT_CONNECT_BAD_CLIENT_ID)
    Serial.print("Connect bad Client-ID");
  else if (MQTTErr == MQTT_CONNECT_UNAVAILABLE)
    Serial.print("Connect unavailable");
  else if (MQTTErr == MQTT_CONNECT_BAD_CREDENTIALS)
    Serial.print("Connect bad credentials");
  else if (MQTTErr == MQTT_CONNECT_UNAUTHORIZED)
    Serial.print("Connect unauthorized");
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
  for(int y=0; y<arrLength; y++) {
    for(int x=0; x<arrWidth; x++) {
      char value = '1';
      testArray[x][y] = value;
      //Serial.print(value);
    }
    //Serial.println("");
  }
}

void sendCheck() {
  if (millis() - timer > 2000) {
    timer = millis(); // reset the timer
    //sendJsonToAWS();
    testSend(); Serial.println("Sent Data");
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
    digitalWrite(LED_PIN, LOW);
    connectToAWS();
  }
  client.loop();
  sendCheck();
}
