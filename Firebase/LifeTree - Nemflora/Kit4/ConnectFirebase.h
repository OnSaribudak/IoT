#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif
#include <addons/RTDBHelper.h>
#define WIFI_SSID "mucitpark"
#define WIFI_PASSWORD "mucitpark388"
#define DATABASE_URL "https://lifetree-2-default-rtdb.europe-west1.firebasedatabase.app/"
#define DATABASE_SECRET "xMccUfxCwlzKyU3F3RP5GLo0s73bSV8Flr9cg06F"


FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

void FirebaseSetting()
{
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  config.database_url = DATABASE_URL;
  config.signer.tokens.legacy_token = DATABASE_SECRET;
  Firebase.reconnectWiFi(true);
  Firebase.begin(&config, &auth);
}


void SendData()
{
  Firebase.setInt(fbdo, "/test/int", 1000);
}

String GetData(String DataPath)
{
   int iVal = 0;
   String a = Firebase.getInt(fbdo, DataPath, &iVal) ? String(iVal).c_str() : fbdo.errorReason().c_str();
   return a;
}
