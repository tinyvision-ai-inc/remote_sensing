#include <MQTT.h>
#include <MQTTClient.h>
#include "WiFi.h"
#include "certs.h"
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

#include <SPI.h>
#include <MFRC522.h>

#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>

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

#define SS_PIN  21  
#define RST_PIN 27   
#define LED_PIN 13 

#define GPSSerial Serial1
//static const int RXPin = 33, TXPin = 15;
//static const uint32_t GPSBaud = 9600;
//SoftwareSerial GPSSerial(RXPin, TXPin);
Adafruit_GPS GPS(&GPSSerial);
uint32_t timer = millis();
String LatLong;
double LAT;
double LONG;


const char* ntpServer = "pool.ntp.org";

MFRC522 rfid(SS_PIN, RST_PIN);

WiFiClientSecure net = WiFiClientSecure();
MQTTClient client = MQTTClient(256);

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

  client.begin(AWS_IOT_ENDPOINT, 8883, net);
  client.onMessage(messageReceived);

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
  reportedObj["LAT"] = LAT;
  reportedObj["LONG"] = LONG;
  //reportedObj["BatteryLevel"] = "79%";
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
  client.publish(AWS_IOT_TOPIC_SHADOW, jsonBuffer);
}

void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
  if(topic=="Tiny-ESP32/lock"){
    if(payload=="false") {
      lockEngage = false;
    }
    if(payload=="true") {
      lockEngage = true;
    }
  }
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

void NFC() {
  String tagID = "";
  if (rfid.PICC_IsNewCardPresent()) { // new tag is available
    if (rfid.PICC_ReadCardSerial()) { // NUID has been readed
      MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
      Serial.print("RFID/NFC Tag Type: ");
      Serial.println(rfid.PICC_GetTypeName(piccType));

      // print UID in Serial Monitor in the hex format
      Serial.print("UID:");
      for (int i = 0; i < rfid.uid.size; i++) {
        //Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
        //Serial.print(rfid.uid.uidByte[i], HEX);
        tagID.concat(String(rfid.uid.uidByte[i], HEX));
      }
      tagID.toUpperCase();
      Serial.println(tagID);
      if(tagID == masterID) {
        lockEngage = false;
      }
      else {
        lockEngage = true;
      }
      rfid.PICC_HaltA(); // halt PICC
      rfid.PCD_StopCrypto1(); // stop encryption on PCD
      
    }
  }
}

void gpsCheck() {
  char c = GPS.read();
  Serial.print(c);
  if (GPS.newNMEAreceived()) {
    if (!GPS.parse(GPS.lastNMEA())){
      return; }
  }
  if (millis() - timer > 2000) {
    timer = millis(); // reset the timer
    Serial.print("Fix: "); Serial.print((int)GPS.fix);
    Serial.print(" quality: "); Serial.println((int)GPS.fixquality);
    Serial.print("Satellites: "); Serial.println((int)GPS.satellites);
    if (GPS.fix) {
      Serial.print("Location: ");
      String latitudeVal = String(GPS.latitudeDegrees, 8);
      LAT = GPS.latitudeDegrees;
      String longitudeVal = String(GPS.longitudeDegrees, 8);
      LONG = GPS.longitudeDegrees;
      //Serial.print("Speed (knots): "); Serial.println(GPS.speed);
      //Serial.print("Angle: "); Serial.println(GPS.angle);
      //Serial.print("Altitude: "); Serial.println(GPS.altitude);
      LatLong = latitudeVal + ", " + longitudeVal;
      Serial.println(LatLong);
    }
    sendJsonToAWS();
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  connectToWiFi();
  connectToAWS();
  client.onMessage(messageReceived);
  configTime(0, 0, ntpServer);
  SPI.begin(); // init SPI bus
  rfid.PCD_Init(); // init MFRC522
  GPS.begin(9600); //init GPS
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  //GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  //GPS.sendCommand(PGCMD_ANTENNA);
  //delay(1000);
  //GPSSerial.println(PMTK_Q_RELEASE);
  Serial.println("Tap an RFID/NFC tag on the RFID-RC522 reader");
  randomSeed(analogRead(A7));
}

void loop() {
  if (!client.connected()) {
    connectToAWS();
  }
  client.loop();
  NFC();
  gpsCheck();
  //delay(100);
  //Serial.println(boolToString(lockEngage));
}
