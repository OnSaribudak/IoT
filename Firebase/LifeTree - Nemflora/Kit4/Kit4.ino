#include "ConnectFirebase.h"
#define MotorA 14
#define MotorB 12
#define MotorPhP 13
#define MotorPhN 15

void setup()
{
  FirebaseSetting();
  pinMode(MotorA,OUTPUT);
  pinMode(MotorB,OUTPUT);
  pinMode(MotorPhP,OUTPUT);
  pinMode(MotorPhN,OUTPUT);
}

void loop()
{ 
 GetData("NemFlora/Kitler/4/Controls/MotorA") == "1" ?  digitalWrite(MotorA,HIGH): digitalWrite(MotorA,LOW);
 GetData("NemFlora/Kitler/4/Controls/MotorB") == "1" ?  digitalWrite(MotorB,HIGH): digitalWrite(MotorB,LOW);
 GetData("NemFlora/Kitler/4/Controls/MotorPh+") == "1" ?  digitalWrite(MotorPhP,HIGH): digitalWrite(MotorPhP,LOW);
 GetData("NemFlora/Kitler/4/Controls/MotorPh-") == "1" ?  digitalWrite(MotorPhN,HIGH): digitalWrite(MotorPhN,LOW);
}
