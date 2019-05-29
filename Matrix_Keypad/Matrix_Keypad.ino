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

#define ledPin 4
#define col1 10
#define col2 12
#define col3 8
#define row1 11
#define row2 6
#define row3 7
#define row4 9
#define pwm 3


unsigned int timer1Start = 65335;
char bounceCount = 0;
unsigned int hist1 = 0;
unsigned int hist2 = 0;
unsigned int hist3 = 0;
unsigned int hist4 = 0;
unsigned int hist5 = 0;
unsigned int hist6 = 0;
unsigned int hist7 = 0;
unsigned int hist8 = 0;
unsigned int hist9 = 0;
unsigned int hist0 = 0;
unsigned int hist10 = 0;
unsigned int hist11 = 0;
unsigned int MASK = 0b1111000000001111;
bool flipped = 0;
char lockout = 0;
char keypress = 13; // this always reads 13 unless a key is pressed.
unsigned int pressed = 0x000F;
unsigned int released = 0xF000;

void setup() {  
  pinMode(ledPin, OUTPUT);
  pinMode(col1, OUTPUT);
  pinMode(col2, OUTPUT);
  pinMode(col3, OUTPUT);
  pinMode(row1, INPUT);
  pinMode(row2, INPUT);
  pinMode(row3, INPUT);
  pinMode(row4, INPUT);
  pinMode(pwm, INPUT);

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
}

void keyRead(){  // Poll each column of the matrix and store the return from each row in the corresponding key's log.
    bounceCount = 0;
    digitalWrite(col1, HIGH);
    hist1 = hist1 << 1;
    hist1 |= digitalRead(row1);
    hist4 = hist4 << 1;
    hist4 |= digitalRead(row2);
    hist7 = hist7 << 1;
    hist7 |= digitalRead(row3);
    hist10 = hist10 << 1;
    hist10 |= digitalRead(row4);
    digitalWrite(col1, LOW);
    
    digitalWrite(col2, HIGH);
    hist2 = hist2 << 1;
    hist2 |= digitalRead(row1);
    hist5 = hist5 << 1;
    hist5 |= digitalRead(row2);
    hist8 = hist8 << 1;
    hist8 |= digitalRead(row3);
    hist0 = hist0 << 1;
    hist0 |= digitalRead(row4);
    digitalWrite(col2, LOW);

    digitalWrite(col3, HIGH);
    hist3 = hist3 << 1;
    hist3 |= digitalRead(row1);
    hist6 = hist6 << 1;
    hist6 |= digitalRead(row2);
    hist9 = hist9 << 1;
    hist9 |= digitalRead(row3);
    hist11 = hist11 << 1;
    hist11 |= digitalRead(row4);
    digitalWrite(col3, LOW);
}


void loop() {
  if (bounceCount >= 50){
    keyRead();
  }
  
  if ((hist0 & MASK) == pressed && !flipped){
    digitalWrite(ledPin, !digitalRead(ledPin)); // Flip-flop the LED
    flipped = 1;
  }
  if ((hist0 & MASK) == released && flipped){
    flipped = 0;
  }
}
