
#include <ESP8266WiFi.h>
#include <espnow.h>
#include "DHT.h"

#define ldr 5
#define tpr 10

#define DHTTYPE DHT11
#define dht_dpin  0     // UNUTMA!!!!

DHT dht(dht_dpin, DHTTYPE);
long int x = 0;
long int y ;

int analogdan_gelen_deger ;
int analogdan_gelen_deger2 ;
float nem_havadaki;
float sicaklik_ortamdaki;

int sure = 2000;

int a, b ;

uint8_t broadcastAddress1[] = {0x3C, 0x61, 0x05, 0xD0, 0xFA, 0x0B}; // 3C:61:05:D0:2E:7C//3C:61:05:D0:61:2B  // UNUTMA!!!! 3C:61:05:D0:FA:0B




typedef struct test_struct {
  int analogdan_gelen_deger;
  int analogdan_gelen_deger2;
  float nem_havadaki;
  float sicaklik_ortamdaki;


} test_struct;

test_struct test;

unsigned long lastTime = 0;
unsigned long timerDelay = 2000;

// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  char macStr[18];

  Serial.print("Packet to:");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
  Serial.print(" send status: ");
  if (sendStatus == 0) {
    Serial.println("Delivery success");
  }
  else {
    Serial.println("Delivery fail");
  }
}





void setup() {

  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  dht.begin();
  pinMode(ldr, OUTPUT);
  pinMode(tpr, OUTPUT);


  Serial.begin(115200);


  WiFi.mode(WIFI_STA);
  WiFi.disconnect();



  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);



  esp_now_add_peer(broadcastAddress1, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);


}

void loop() {


  y = millis();
  if (y - x <= sure) {

    digitalWrite(ldr, HIGH);
    analogdan_gelen_deger = analogRead(A0);
    test.analogdan_gelen_deger = analogRead(A0);
    Serial.print("ldr \n");
    Serial.println(analogdan_gelen_deger);

  }
  else if (y - x > sure && y - x <= (sure * 2)) {



    digitalWrite(ldr, LOW);

    digitalWrite(tpr, HIGH);
    analogdan_gelen_deger2 = analogRead(A0);
    test.analogdan_gelen_deger2 = analogRead(A0);
    Serial.print("tpr\t");
    Serial.println(analogdan_gelen_deger2);

  }

  else {
    digitalWrite(tpr, LOW);
    x = millis();
  }



  nem_havadaki = dht.readHumidity();
  sicaklik_ortamdaki = dht.readTemperature();
  Serial.print("\t\t\t");
  Serial.print("Havadaki Nem Oranı = ");
  Serial.print(nem_havadaki);
  Serial.print("%  ");
  Serial.print("Ortamın Sıcaklığı = ");
  Serial.print(sicaklik_ortamdaki);
  Serial.println("C  ");

  delay(100);


  if ((millis() - lastTime) > timerDelay) {


    //    test.analogdan_gelen_deger = analogRead(A0);
    //    test.analogdan_gelen_deger2 = analogRead(A0);
    test.nem_havadaki = dht.readHumidity();
    test.sicaklik_ortamdaki = dht.readTemperature();


    esp_now_send(0, (uint8_t *) &test, sizeof(test));

    lastTime = millis();
  }

}
