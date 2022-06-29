#include <DallasTemperature.h>
#include <OneWire.h>

#define ONE_WIRE_BUS 15                       
float water_temp;
OneWire oneWire(ONE_WIRE_BUS);
 
DallasTemperature sensors(&oneWire);           

void Water_Temp_Setup()
{
  Serial.begin(115200); 
  sensors.begin();
}

void Water_Temp_Loop()
{ 
  sensors.requestTemperatures();  
  water_temp = sensors.getTempCByIndex(0);             
  return ;
}
