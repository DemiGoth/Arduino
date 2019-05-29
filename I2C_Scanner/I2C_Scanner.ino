#include <Wire.h>

void setup() {
  Serial.begin (9600);
  while (!Serial) 
    {
    }
  Serial.println ();
  Serial.println ("Finding I2C Address ...");
  byte count = 0;
  
  Wire.begin();
  for (byte i = 8; i <120; i++)
  {
    Wire.beginTransmission (i);
    if (Wire.endTransmission () == 0)
      {
      Serial.println ("Yey, Got it!");
      Serial.print ("Address is : ");
      Serial.print ("0x");
      Serial.println (i, HEX);
      count++;
      delay (1);  
      } 
  } 
  Serial.println ("Done.");
  Serial.print ("Get it ");
  Serial.print (count, DEC);
  Serial.println (" Device");
} 
void loop() {}
