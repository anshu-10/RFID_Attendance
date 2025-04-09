 /*
 CODE CREATED BY
 Anshu Jamare
 Zaid Khan 
 Anshu Jamare
 */

#include <SPI.h>
#include <MFRC522.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>

//----------------------------
// PIN Definitions
#define RST_PIN  D3
#define SS_PIN   D4
#define BUZZER   D2
//----------------------------
MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;  
MFRC522::StatusCode status;      

int blockNum = 2;  
byte bufferLen = 18;
byte readBlockData[18];

//----------------------------
// YOUR WiFi Credentials 👇
#define WIFI_SSID "Anshu"
#define WIFI_PASSWORD "12345678"

// Google Apps Script URL
const String sheet_url = "https://script.google.com/macros/s/AKfycbzR9evMKQVC-joWJXUOjJaZjt9Fyl_dtVBssFc3fOP35s9etbrBMHFWfQP7Gyx5FeGXWg/exec?name=";
//----------------------------

void setup()
{
  Serial.begin(9600);
  delay(1000);
  Serial.println("\nConnecting to WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  // WiFi Connecting
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi ✅");
  Serial.println("IP Address: " + WiFi.localIP().toString());

  pinMode(BUZZER, OUTPUT);
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("Place your RFID card on the reader...");
}

void loop()
{
  if ( ! mfrc522.PICC_IsNewCardPresent()) return;
  if ( ! mfrc522.PICC_ReadCardSerial()) return;

  Serial.println("\nCard Detected:");
  String uidStr = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    uidStr += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
    uidStr += String(mfrc522.uid.uidByte[i], HEX);
  }
  uidStr.toUpperCase();
  Serial.println("UID: " + uidStr);

  // Read Data from Block
  ReadDataFromBlock(blockNum, readBlockData);
  String rfidData = String((char*)readBlockData);
  rfidData.trim();
  Serial.println("Card Data: " + rfidData);

  // Buzz
  digitalWrite(BUZZER, HIGH); delay(150);
  digitalWrite(BUZZER, LOW);  delay(150);
  digitalWrite(BUZZER, HIGH); delay(150);
  digitalWrite(BUZZER, LOW);

  // WiFi Request
  if (WiFi.status() == WL_CONNECTED) {
    std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);
    client->setInsecure();  // Skip fingerprint checking

    String finalURL = sheet_url + rfidData;
    HTTPClient https;

    Serial.println("Sending data to Google Sheet...");
    if (https.begin(*client, finalURL)) {
      int httpCode = https.GET();
      if (httpCode > 0) {
        Serial.printf("HTTP Response Code: %d\n", httpCode);
        String payload = https.getString();
        Serial.println("Server Response: " + payload);
      } else {
        Serial.printf("GET Failed: %s\n", https.errorToString(httpCode).c_str());
      }
      https.end();
    } else {
      Serial.println("Unable to connect to Google Apps Script.");
    }
  }
  delay(3000); // Wait before next read
}

void ReadDataFromBlock(int blockNum, byte readBlockData[]) 
{
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockNum, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print("Authentication failed: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  status = mfrc522.MIFARE_Read(blockNum, readBlockData, &bufferLen);
  if (status != MFRC522::STATUS_OK) {
    Serial.print("Reading failed: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  Serial.println("RFID Block Read Success ✅");
}