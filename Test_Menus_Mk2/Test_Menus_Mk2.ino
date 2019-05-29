/*
Matrix pin layout:
Pin 1 = Col 2 (Left most pin when looking at the number side)
Pin 2 = Row 1 (Top row)
Pin 3 = Col 1 (Left most col)
Pin 4 = Row 4
Pin 5 = Col 3
Pin 6 = Row 3
Pin 7 = Row 2
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define col1 10 // name all the useful pins
#define col2 12
#define col3 8
#define row1 11
#define row2 6
#define row3 7
#define row4 9

LiquidCrystal_I2C lcd(0x27, 20, 4); // This HAS to be outside the setup function. (I don't know why.)

// Timer1 variables
unsigned int timer1Start = 65335;
byte bounceCount = 0;

// Button variables
const byte numberofbuttons = 12;
unsigned int history [numberofbuttons] = {0};
byte ispressed [numberofbuttons] = {0};
unsigned int holdtime [numberofbuttons] = {0};
unsigned int MASK = 0b1111000000001111;  // button states and debounce MASK
unsigned int pressed = 0x000F;
unsigned int released = 0xF000;
unsigned int unpressed = 0x0000;
unsigned int held = 0xF00F;
unsigned int longpresstime = 5000;

// Screen variables
const byte numberofscreens = 3;
byte currentscreen = 0;
unsigned int userinput [numberofscreens] = {0};
String screens [numberofscreens][2][4] = { // Screen, column, row. 
  {{"Scrn 0","02","03","04"},{"Demi Systems","05","06","07"}},
  {{"Scrn 1","12","13","14"},{"Demi Systems","15","16","17"}},
  {{"Scrn 2","23","23","24"},{"Demi Systems","25","26","27"}}
};

void setup() {  
  pinMode(col1, OUTPUT);  // keypad pin setup
  pinMode(col2, OUTPUT);
  pinMode(col3, OUTPUT);
  pinMode(row1, INPUT);
  pinMode(row2, INPUT);
  pinMode(row3, INPUT);
  pinMode(row4, INPUT);

  noInterrupts();
  TCCR1A = 0;
  TCCR1B = B00000010; // Prescale Timer1 to 8
  TCNT1 = timer1Start; // Timer1 is set to interupt every 0.0001 seconds
  TIMSK1 = B00000001; // Enable timer1 Overflow Interupt
  interrupts();

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.print(screens[currentscreen][0][0]); // column 1
  lcd.setCursor(0,1);
  lcd.print(screens[currentscreen][0][1]);
  lcd.setCursor(0,2);
  lcd.print(screens[currentscreen][0][2]);
  lcd.setCursor(0,3);
  lcd.print(screens[currentscreen][0][3]);
  lcd.setCursor(8,0);
  lcd.print(screens[currentscreen][1][0]); // column 2
  lcd.setCursor(8,1);
  lcd.print(screens[currentscreen][1][1]);
  lcd.setCursor(8,2);
  lcd.print(screens[currentscreen][1][2]);
  lcd.setCursor(8,3);
  lcd.print(screens[currentscreen][1][3]);
}


ISR(TIMER1_OVF_vect) { // Timer1 is set to interupt every 0.0001 seconds
  
  TCNT1 = timer1Start; // Set the timer start value as defined in setup()

  bounceCount++;
}


void updateKeyHistory(){  // Poll each column of the matrix and store the return from each row in the corresponding key's log.
    bounceCount = 0;
    digitalWrite(col1, HIGH);
    history[1] = history[1] << 1;
    history[1] |= digitalRead(row1);
    history[4] = history[4] << 1;
    history[4] |= digitalRead(row2);
    history[7] = history[7] << 1;
    history[7] |= digitalRead(row3);
    history[10] = history[10] << 1; // Star (*) button
    history[10] |= digitalRead(row4);
    digitalWrite(col1, LOW);
    digitalWrite(col2, HIGH);
    history[2] = history[2] << 1;
    history[2] |= digitalRead(row1);
    history[5] = history[5] << 1;
    history[5] |= digitalRead(row2);
    history[8] = history[8] << 1;
    history[8] |= digitalRead(row3);
    history[0] = history[0] << 1;
    history[0] |= digitalRead(row4);
    digitalWrite(col2, LOW);
    digitalWrite(col3, HIGH);
    history[3] = history[3] << 1;
    history[3] |= digitalRead(row1);
    history[6] = history[6] << 1;
    history[6] |= digitalRead(row2);
    history[9] = history[9] << 1;
    history[9] |= digitalRead(row3);
    history[11] = history[11] << 1; // Hash (#) button
    history[11] |= digitalRead(row4);
    digitalWrite(col3, LOW);
}

void inputclear(){
  for (byte i = 0; i < numberofbuttons; i++){
    ispressed[i] = 0;
  }
  return;
}

void updateLCD(){
  lcd.clear();
  lcd.print(screens[currentscreen][0][0]); // column 1
  lcd.setCursor(0,1);
  lcd.print(screens[currentscreen][0][1]);
  lcd.setCursor(0,2);
  lcd.print(screens[currentscreen][0][2]);
  lcd.setCursor(0,3);
  lcd.print(screens[currentscreen][0][3]);
  lcd.setCursor(8,0);
  lcd.print(screens[currentscreen][1][0]); // column 2
  lcd.setCursor(8,1);
  lcd.print(screens[currentscreen][1][1]);
  lcd.setCursor(8,2);
  lcd.print(screens[currentscreen][1][2]);
  lcd.setCursor(8,3);
  lcd.print(screens[currentscreen][1][3]);
  inputclear();
  return;
}

void updateMenu(){
  if(ispressed[10] == 2){
    if(currentscreen == 0){
      currentscreen = numberofscreens - 1;
    }
    else{
      currentscreen--;
    }
  }
  else if(ispressed[11] == 2){
    if(currentscreen == numberofscreens - 1){
      currentscreen = 0;
    }
    else{
      currentscreen++;
    }
  }
  updateLCD();
  return;
}

void keyRead(){
  for (byte i = 0; i < numberofbuttons; i++){
    if((history[i] & MASK) == pressed && !ispressed[i]){
      ispressed[i] = 1;
      holdtime[i] = 0;
    }
    else if((history[i] & MASK) == released && ispressed[i] == 1 && holdtime[i] < longpresstime){
      ispressed[i] = 2;
      holdtime[i] = 0;
    }
/*  else if ((history[i] & MASK) == held && ispressed[i] == 1 && holdtime[i] >= longpresstime){
      star.ispressed = 3;
      star.holdtime = 0;
      // go somewhere else
    }*/
  }
  return;
}

void loop() {
  if (bounceCount >= 20){
    updateKeyHistory();
    keyRead();
  }
  if (ispressed[10] | ispressed[11]){
    updateMenu();
  }
}
