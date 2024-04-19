#include <Wire.h>
int val11;
float val2;
 
void setup()
{
  Serial.begin(57600);
  Serial.println("Voltage: ");
  Serial.print("V");
}
void loop()
{
  float temp;
  val11=analogRead(A0);
  temp=val11/4.092;
  val2=(temp/10);
  Serial.println(val2);
    
   delay(1000);
}
