#include <espnow.h>
#include <EEPROM.h>
#include "GravityTDS.h"
#include <DallasTemperature.h>
#include <OneWire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


int sure = 10000;
long int x = 0;
long int y ;

#define TdsSensorPin A0
#define ONE_WIRE_BUS 15
#define Tds_pin  12
#define Ph_pin   16


float calibration_value = 21.34 - 0.7;
int phval = 0; 
unsigned long int avgval; 
int buffer_arr[10],temp;
float ph_act;

int ec;
float water_temp;
float tdsValue = 0;


unsigned long previousMillis = 0;   
const long interval = 10000;        

OneWire oneWire(ONE_WIRE_BUS);
 
DallasTemperature sensors(&oneWire);

GravityTDS gravityTds;


void setup()
{
  Serial.begin(115200);
  gravityTds.setPin(TdsSensorPin);
  gravityTds.setAref(3.3);  //reference voltage on ADC, default 5.0V on Arduino UNO
  gravityTds.setAdcRange(1024);  //1024 for 10bit ADC;4096 for 12bit ADC
  gravityTds.begin();  //initialization
  sensors.begin();
    
  pinMode(Tds_pin,OUTPUT);
  pinMode(Ph_pin,OUTPUT);
}


void loop()
{
  y = millis();
  if (y - x <= sure)
  {
    Tds_Temp_sensor();
  }

   else if (y - x > sure && y - x <= (sure * 2)) 
   {
    Ph_Sensor();
   }
   else 
   {
    x = millis(); 
   }  
}


void Ph_Sensor ()
{
  digitalWrite(Tds_pin,LOW);
  digitalWrite(Ph_pin,HIGH);
  
  for(int i=0;i<10;i++) 
  {
    buffer_arr[i]=analogRead(A0);
    delay(30);
  }
  
  for(int i=0;i<9;i++)
  {
    for(int j=i+1;j<10;j++)
    {
      if(buffer_arr[i]>buffer_arr[j])
      {
        temp=buffer_arr[i];
        buffer_arr[i]=buffer_arr[j];
        buffer_arr[j]=temp;
      }
    }
  }
  
  avgval=0;
  for(int i=2;i<8;i++) avgval+=buffer_arr[i];
  float volt=(float)avgval*3.3/1024/6; 
  ph_act = -5.70 * volt + calibration_value;
 
 Serial.print("pH Val: ");
 Serial.println(ph_act);
 delay(1000);
 
}



void Tds_Temp_sensor ()
{
  sensors.requestTemperatures();  
  water_temp = sensors.getTempCByIndex(0);              
  Serial.print("Su sıcaklığı : ");
  Serial.println(water_temp);
  delay(500); 
  digitalWrite(Ph_pin,LOW);
  digitalWrite(Tds_pin,HIGH);
  gravityTds.setTemperature((sensors.getTempCByIndex(0))-2);  // set the temperature and execute temperature compensation
  gravityTds.update();  //sample and calculate 
  tdsValue = gravityTds.getTdsValue();  // then get the value
  Serial.print("EC : ");
  ec = (tdsValue*2);
  Serial.println(ec,0);
}

void analog_read ()
{ 
  
}
  
