#define ledPin 6

unsigned int timer1Start = 65335;
char bounceCount = 0;
unsigned int sw12hist = 0;
unsigned int MASK = 0b1111000000001111;
unsigned int sw12masked = 0;
bool flipped = 0;

void setup() {
  
  pinMode(ledPin, OUTPUT); //ledPin
  pinMode(12, INPUT_PULLUP); // switchPin

  noInterrupts();
  TCCR1A = 0;
  TCCR1B = B00000010; // Prescale Timer1 to 8
  TCNT1 = timer1Start; // Timer1 is set to interupt every 0.0001 seconds
  TIMSK1 = B00000001; // Enable timer1 Overflow Interupt
  interrupts();
  
}

ISR(TIMER1_OVF_vect) { // Timer1 is set to interupt every 0.0001 seconds
  
  TCNT1 = timer1Start; // Set the timer start value as defined in setup()

  bounceCount++;
  if (bounceCount >= 50){
    bounceCount = 0;
    sw12hist = sw12hist << 1;
    sw12hist |= digitalRead(12);
  }
}


void loop() {
  
  if ((sw12hist & MASK) == 0xF000 && !flipped){
    digitalWrite(ledPin, !digitalRead(ledPin)); // Flip-flop the LED
    flipped = 1;
  }
  else if ((sw12hist & MASK) == 0x000F && flipped){
    flipped = 0;
  }
}










/*
void keyRead(){
  sw12masked = sw12hist & MASK;
  
  if (sw12masked == 0xF000){
    sw12hist = 0xFFFF;
    pressed = 12;
  }
  else if (sw12masked == 0x0000){
    down = 12;
  }
  else if (sw12masked == 0x000F){
    sw12hist = 0x0000;
    released = 12;
  }
  else if (sw12masked == 0xF00F){
    up = 12;
  }
  return;
}
*/
