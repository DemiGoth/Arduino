#define ledPin 2
#define ledPinRed 6
#define potPin A0

int timer1Start = 65336;
int A0Read = 0;
int count = 0;
byte A0Count = 0;

void setup() {
  pinMode(ledPin, OUTPUT); //ledPin
  pinMode(ledPinRed, OUTPUT);
  
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = B00000010; // Prescale Timer1 to 8
  TCNT1 = timer1Start;
  TIMSK1 = B00000001; // Enable timer1 Overflow Interupt
  interrupts();  
}

ISR(TIMER1_OVF_vect) { // Timer1 is set to interupt every 0.0001 seconds
  
  TCNT1 = timer1Start; // Set the timer start value as defined in setup()
  
  A0Count++;
  if (A0Count >= 200){
    A0Read = analogRead(A0); // poll the ADC every 0.02 seconds
    A0Count = 0;
    count++;
  }
}

void loop() {
  if (count >= A0Read * 2) {
    count = 0;
    if (digitalRead(ledPin)){
      digitalWrite(ledPin, 0);
      digitalWrite(ledPinRed, 1);
    }
    else{
      digitalWrite(ledPin, 1);
      digitalWrite(ledPinRed, 0);
    }
  }  
}
