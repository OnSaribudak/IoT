#include "Ec_WaterTemp.h"
#include "PH_sensor.h"
#include "ConnectFirebase.h"

long int x = 0;
long int y ;


void setup()
{
 Ec_WaterTemp_Setup();
 Ph_Setup() ;
 FirebaseSetting();
}

void loop()
{
  AnalagRead(); /////////////// İKİ TANE ANALOG OKUMA YAPIYORSUNN!!!!!!!!!!!!!!!!!!!!!
}


void AnalagRead()
{
  y = millis();
  if (y - x <= 10000) 
  {
   float a= random(400,500);
   float b= random(350,370);
    
    Ec_WaterTemp_Loop();
    SendData("NemFlora/Kitler/2/RealTimeData/Ec", Ec_WaterTemp_Loop());
        SendData("NemFlora/Kitler/2/RealTimeData/Nem", a/10 );
        SendData("NemFlora/Kitler/2/RealTimeData/Şeker", b/10 );
        
  // GetData("NemFlora/Kitler/4/Controls/MotorA") == "1" ?SendData("NemFlora/Kitler/2/RealTimeData/Olgunluk", 1 ):SendData("NemFlora/Kitler/2/RealTimeData/Olgunluk",0);
    Water_Temp_Loop();


  }
  else if (y - x > 10000 && y - x <= 20000)
  {
    float a= random(400,500);
   float b= random(350,370);
    
   Ph_Loop();
   SendData("NemFlora/Kitler/2/RealTimeData/Ph", Ph_Loop());
   SendData("NemFlora/Kitler/2/RealTimeData/Nem", a/10);
   SendData("NemFlora/Kitler/2/RealTimeData/Şeker", b/10 );
   // GetData("NemFlora/Kitler/4/Controls/MotorA") == "1" ?SendData("NemFlora/Kitler/2/RealTimeData/Olgunluk", 1 ):SendData("NemFlora/Kitler/2/RealTimeData/Olgunluk",0);
   
   Water_Temp_Loop();

  }

  else {
     //GetData("NemFlora/Kitler/4/Controls/MotorA") == "1" ?SendData("NemFlora/Kitler/2/RealTimeData/Olgunluk", 1 ):SendData("NemFlora/Kitler/2/RealTimeData/Olgunluk",0);
   
    
    x = millis();
  }

}
