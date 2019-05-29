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

LiquidCrystal_I2C lcd(0x27, 20, 4); // This HAS to be outside the setup function.

unsigned int timer1Start = 65335; // Timer1 variables
char bounceCount = 0;

struct button{ // All the variables for the keypad.
  unsigned int history;
  unsigned int holdtime;
  char ispressed;
  };
button zero, one, two, three, four, five, six, seven, eight, nine, star, hash;

unsigned int MASK = 0b1111000000001111;  // button states and debounce MASK
unsigned int pressed = 0x000F;
unsigned int released = 0xF000;
unsigned int unpressed = 0x0000;
unsigned int held = 0xF00F;
unsigned int longpresstime = 5000;

const int numberofscreens = 3;
unsigned int currentscreen = 0;
unsigned int lastscreen = 0;
String screens [numberofscreens][2][4] = { // Screen, column, row. 
  {{"Scrn 0","02","03","04"},{"Demi Systems","05","06","07"}},
  {{"Scrn 1","12","13","14"},{"Demi Systems","15","16","17"}},
  {{"Scrn 2","22","23","24"},{"Demi Systems","25","26","27"}}
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
  
  /*if (star.ispressed == 1){
    star.holdtime++;
  }
  if (hash.ispressed == 1){
    hash.holdtime++;
  } */
}


void updateKeyHistory(){  // Poll each column of the matrix and store the return from each row in the corresponding key's log.
    bounceCount = 0;
    digitalWrite(col1, HIGH);
    one.history = one.history << 1;
    one.history |= digitalRead(row1);
    four.history = four.history << 1;
    four.history |= digitalRead(row2);
    seven.history = seven.history << 1;
    seven.history |= digitalRead(row3);
    star.history = star.history << 1;
    star.history |= digitalRead(row4);
    digitalWrite(col1, LOW);
    digitalWrite(col2, HIGH);
    two.history = two.history << 1;
    two.history |= digitalRead(row1);
    five.history = five.history << 1;
    five.history |= digitalRead(row2);
    eight.history = eight.history << 1;
    eight.history |= digitalRead(row3);
    zero.history = zero.history << 1;
    zero.history |= digitalRead(row4);
    digitalWrite(col2, LOW);
    digitalWrite(col3, HIGH);
    three.history = three.history << 1;
    three.history |= digitalRead(row1);
    six.history = six.history << 1;
    six.history |= digitalRead(row2);
    nine.history = nine.history << 1;
    nine.history |= digitalRead(row3);
    hash.history = hash.history << 1; // Hash (#) button
    hash.history |= digitalRead(row4);
    digitalWrite(col3, LOW);
}

void inputclear(){
  zero.ispressed = 0;
  one.ispressed = 0;
  two.ispressed = 0;
  three.ispressed = 0;
  four.ispressed = 0;
  five.ispressed = 0;
  six.ispressed = 0;
  seven.ispressed = 0;
  eight.ispressed = 0;
  nine.ispressed = 0;
  star.ispressed = 0;
  hash.ispressed = 0;
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
  lastscreen = currentscreen;
  inputclear();
  return;
}

void updateMenu(){
  if(star.ispressed == 2){
    if(currentscreen == 0){
      currentscreen = numberofscreens - 1;
    }
    else{
      currentscreen--;
    }
  }
  else if(hash.ispressed == 2){
    if(currentscreen == numberofscreens - 1){
      currentscreen = 0;
    }
    else{
      currentscreen++;
    }
  }
  return;
}

void keyRead(){
  if((star.history & MASK) == pressed && !star.ispressed){
    star.ispressed = 1;
    star.holdtime = 0;
  }
/*  else if((star.history & MASK) == held && star.ispressed == 1 && star.holdtime >= longpresstime){
    star.ispressed = 3;
    star.holdtime = 0;
    return;
  }*/
  else if((star.history & MASK) == released && star.ispressed == 1 && star.holdtime < longpresstime){
    star.ispressed = 2;
    star.holdtime = 0;
    updateMenu();
  }
  
  if((hash.history & MASK) == pressed && !hash.ispressed){
    hash.ispressed = 1;
    hash.holdtime = 0;
  }
  else if((hash.history & MASK) == released && hash.ispressed == 1 && hash.holdtime < longpresstime){
    hash.ispressed = 2;
    hash.holdtime = 0;
  }
  return;
}

void loop() {
  if (bounceCount >= 20){
    updateKeyHistory();
    keyRead();
  }
  if (zero.ispressed | one.ispressed | two.ispressed | three.ispressed | four.ispressed | five.ispressed | six.ispressed | seven.ispressed | eight.ispressed | nine.ispressed | star.ispressed | hash.ispressed){
    updateMenu();
  }
  if (lastscreen != currentscreen){
    updateLCD();
  }
}
