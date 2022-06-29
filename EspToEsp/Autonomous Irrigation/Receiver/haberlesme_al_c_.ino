#include <espnow.h>
#include <ESP8266WiFi.h>

#define relay 5

int m_esik = 100 ;

// Structure example to receive data
// Must match the sender structure
typedef struct test_struct {
  int analogdan_gelen_deger ;
  int  analogdan_gelen_deger2;
  float nem_havadaki;
  float sicaklik_ortamdaki;

} test_struct;

// Create a struct_message called myData
test_struct myData;



// Callback function that will be executed when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("ldr: ");
  Serial.println(1024-myData.analogdan_gelen_deger);
  Serial.print("tpr: ");
  Serial.println(1024-myData.analogdan_gelen_deger2);
  Serial.println();
  Serial.print("nem: ");
  Serial.println(myData.nem_havadaki);
  Serial.print("sıcaklk: ");
  Serial.println(myData.sicaklik_ortamdaki);
}

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  
  pinMode(relay, OUTPUT);
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info

  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {


  MotorControl(1024-myData.analogdan_gelen_deger2);

}

void MotorControl(int toprakDeger)
{
  digitalWrite(relay, toprakDeger <= m_esik);
}
