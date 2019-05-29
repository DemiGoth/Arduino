#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

unsigned int timer1Start = 65335;

void setup() {

  noInterrupts();
  TCCR1A = 0;
  TCCR1B = B00000010; // Prescale Timer1 to 8
  TCNT1 = timer1Start; // Timer1 is set to interupt every 0.0001 seconds
  TIMSK1 = B00000001; // Enable timer1 Overflow Interupt

  lcd.init();
  lcd.backlight();
  
}

ISR(TIMER1_OVF_vect) { // Timer1 is set to interupt every 0.0001 seconds
  
  TCNT1 = timer1Start; // Set the timer start value as defined in setup()

}



void loop() {
  lcd.setCursor(0,0);
  lcd.print("Hello World!");
  delay(2000);
  lcd.setCursor(0,0);
  lcd.print("This WORKS!");
  delay(2000);
}
