#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>

//#define GPSSerial Serial1

#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN  21  
#define RST_PIN 27  
MFRC522 rfid(SS_PIN, RST_PIN);

static const int RXPin = 33, TXPin = 15;
static const uint32_t GPSBaud = 9600;

TinyGPSPlus gps;

SoftwareSerial GPSSerial(RXPin, TXPin);

void setup() {
  // put your setup code here, to run once:
  while (!Serial);

  // make this baud rate fast enough to we aren't waiting on it
  Serial.begin(115200);

  // 9600 baud is the default rate for the Ultimate GPS
  GPSSerial.begin(GPSBaud);
  SPI.begin(); // init SPI bus
  rfid.PCD_Init(); // init MFRC522
  //GPSSerial.begin(9600);
}

void displayInfo()
{
  Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" "));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.println();
}

void loop() {
  // put your main code here, to run repeatedly:
  //bool test = rfid.PICC_IsNewCardPresent();
  if (GPSSerial.available() > 0)
    if (gps.encode(GPSSerial.read()))
      displayInfo();

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }
}
