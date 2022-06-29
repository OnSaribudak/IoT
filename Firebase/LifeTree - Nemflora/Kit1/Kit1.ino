#include "ConnectFirebase.h"
#include "DHT.h"
#define DHTTYPE DHT11
#define dht_dpin 14 
DHT dht(dht_dpin, DHTTYPE); 

void setup()
{
  FirebaseSetting();
  dht.begin();
}

void loop()
{   
  int h = dht.readHumidity();
  int t = dht.readTemperature(); 
  SendData("NemFlora/Kitler/1/RealTimeData/Humidity", h);
  SendData("NemFlora/Kitler/1/RealTimeData/Tempature", t);
  Serial.print(h);
  Serial.println(t);
  
}
