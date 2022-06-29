

#include <Arduino.h>
#include "EEPROM.h"
#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>
#include "DHTesp.h"
DHTesp dht;
#define ldr 5
#define tpr 4
long int x = 0;
long int y;
int analogdan_gelen_deger1;
int analogdan_gelen_deger2;
int sure = 2000;

#define EEPROM_SIZE 512

int a, b;

unsigned long lastTime = 0;
unsigned long timerDelay = 10000;

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "mucitpark"
#define WIFI_PASSWORD "mucitpark388"

// Insert Firebase project API Key
#define API_KEY "AIzaSyB5IqG0nF7W4fDaiVy60uKkaw7UhcAzLM8"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "lifetree-2-default-rtdb.europe-west1.firebasedatabase.app"

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;
void sendSensorValue(String kitID, String dateTime, float humidityValue, float temperatureValue, int soilHumidityValue);
void sendSensorRealtimeValue(String kitID,  float humidityValue, float temperatureValue, int soilHumidityValue);

void setup()
{
  Serial.begin(9600);
  EEPROM.begin(EEPROM_SIZE);
   Serial.setTimeout(2000);
  pinMode(ldr, OUTPUT);
  pinMode(tpr, OUTPUT);

  dht.setup(14, DHTesp::DHT11); // Connect DHT sensor to GPIO 14

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", ""))
  {
    Serial.println("ok");
    signupOK = true;
  }
  else
  {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

String BrandName = "NemFlora";
String KitsTable = "Kitler";
String KitID = "1";
String RealtimeKey = "RealtimeData";
String DatasKey = "Datas"; 
String HumidityKey = "Humidity";
String TemperatureKey = "Temperature";
String SoilHumidityKey = "SoilHumidity";


void loop()
{

  
  a = EEPROM.read(0);
  lastTime=millis();
     a=a+1;

  EEPROM.write(0, a);
  String b ="";

  EEPROM.commit();
  digitalWrite(tpr, HIGH);
  float HumidityValue = dht.getHumidity();
  float TemperatureValue = dht.getTemperature();
  int SoilHumidityValue = analogRead(A0);
  sendSensorValue(KitID,b+a,HumidityValue,TemperatureValue,SoilHumidityValue);
  //sendSensorRealtimeValue(KitID,HumidityValue,TemperatureValue,SoilHumidityValue);
  Serial.println(SoilHumidityValue);
  Serial.println(TemperatureValue);
  Serial.println(HumidityValue);
  delay(100);
  ESP.deepSleep(10800e6); 
}

void sendSensorValue(String kitID, String dateTime, float humidityValue, float temperatureValue, int soilHumidityValue)
{
  Firebase.RTDB.setInt(&fbdo,BrandName+"/"+KitsTable+"/"+kitID+"/"+DatasKey+"/"+dateTime+"/"+HumidityKey, 40);
  Firebase.RTDB.setFloat(&fbdo,BrandName+"/"+KitsTable+"/"+kitID+"/"+DatasKey+"/"+dateTime+"/"+TemperatureKey, 27);
  Firebase.RTDB.setInt(&fbdo,BrandName+"/"+KitsTable+"/"+kitID+"/"+DatasKey+"/"+dateTime+"/"+SoilHumidityKey, 50);
 
}


void sendSensorRealtimeValue(String kitID,  float humidityValue, float temperatureValue, int soilHumidityValue)
{
  
   Firebase.RTDB.setInt(&fbdo,BrandName+"/"+KitsTable+"/"+kitID+"/"+RealtimeKey+"/"+HumidityKey, humidityValue);
   Firebase.RTDB.setFloat(&fbdo,BrandName+"/"+KitsTable+"/"+kitID+"/"+RealtimeKey+"/"+TemperatureKey, temperatureValue);
   Firebase.RTDB.setInt(&fbdo,BrandName+"/"+KitsTable+"/"+kitID+"/"+RealtimeKey+"/"+SoilHumidityKey, soilHumidityValue);

}

