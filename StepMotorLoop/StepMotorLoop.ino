
#include <avr/interrupt.h>
#include <Micros.h>
#include <Millis.h>
#include "config.h"


void setup() {
  cli();

  //Step Motor controller pins to output
  DDRC = 0b00000111;

  //Set input pins (step_in, dir_in and en_in) and output pin (LED)
  DDRB =  0b00100000;
  PORTB = 0b00000111;

  //Set input pins (A, B).
  DDRD =  0b00000000;
  PORTD = 0b00110000;

  cli();//Disable interrupts
  //See https://sites.google.com/site/qeewiki/books/avr-guide/external-interrupts-on-the-atmega328


  //enable interupts for quadrature signals + EN&STEP
  PCICR = 0b00000101; //Enable PCIE0 PCIE2

  PCMSK0 = 0b00000101; //Pin Change Mask Register. Enable Pin change interrupt for PCINT02 (EN pin) & PCINT00 (STEP_IN)
  PCMSK2 = 0b00110000; //Pin Change Mask Register. Enable Pin change interrupt for PCINT20&PCINT21 (IN_A,IN_B)

  //initialize ports
  PORTC = PINB;

  sei();//Enable interrupts

  Serial.begin(115200);
  Serial.println("Starting...");
}

long int volatile encoder_position = 0;
long int volatile in_stepCounter = 0;

bool motorJamming = false;
bool errorHappened = false;

int errSteps;

//Handle EN_PIN & STEP_IN change interrupt
ISR (PCINT0_vect)
{
  u8 pinb = PINB;
  u8 stepIn = pinb & 0b00000001; //much faster than digitalRead()
  u8 dirIn =  pinb & 0b00000010;
  u8 enIn =   pinb & 0b00000100;

  if (!motorJamming || enIn ) {
    PORTC = pinb; //Forward StepIn,DirIn,EnIn to stepmotor controller
  }

  if (stepIn && !enIn) { //Rising edge
    if (dirIn) {
      in_stepCounter++;
    } else {
      in_stepCounter--;
    }
  }
}

//Handle quadrature signals
ISR (PCINT2_vect)
{
  //Copied from https://reprap.org/wiki/Magnetic_Rotary_Encoder_v1.0
  u8 pind = PIND;
  u8 in_a = pind & 0b00010000;
  u8 in_b = pind & 0b00100000;

  // found a low-to-high on channel A
  if (in_a)
  {
    // check channel B to see which way
    if (in_b)
      encoder_position--;
    else
      encoder_position++;
  }
  // found a high-to-low on channel A
  else
  {
    // check channel B to see which way
    if (in_b)
      encoder_position++;
    else
      encoder_position--;
  }
}


int calculateError() {
  long int volatile a;
  long int volatile i;

  cli();
  a = encoder_position;
  i = in_stepCounter;
  sei();

  return (a) - ((ENCODER_PPR ) * i) / MOTOR_PPR;
}

int checkErrorDirection() {
  static bool errorFound = false;
  int m = calculateError();

  if (m < -10 || m > 10) {
    errorFound = true;
  }

  if (m >= -5 && m <= 5) {
    errorFound = false;
  }

  if (errorFound) {
    if (m < 0) {
      return 1;
    } else {
      return 2;
    }
  }
  return 0; //OK
}

void printDebugInfo() {
  static Millis t = Millis(100);
  if (t.check()) {
    t.reset();

    Serial.print(in_stepCounter);
    Serial.print(",");
    Serial.println(encoder_position);
  }
}

void loop() {
  //digitalWrite(MOTOR_EN, digitalRead(IN_EN));
  // runMotor();
  runLed();
  // runMotor();
  // printDebugInfo();
  runMotor();
}
