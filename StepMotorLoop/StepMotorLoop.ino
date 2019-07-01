
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

  //Set input pins (A, B) and HOLD_ON (as open state).
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

  #if ENABLE_PRINTS>0
    Serial.begin(115200);
    Serial.println("Starting...");
  #endif
}

long long int volatile internal_encoder_position = 0; //Use only in interrupt functions
long long int volatile internal_in_stepCounter = 0;   //Use only in interrupt functions

long long int volatile public_encoder_position = 0;
long long int volatile public_in_stepCounter = 0;


bool motorJamming = false;
bool errorHappened = false;

int errSteps;

volatile u8 lastStepInState=0;

//Handle EN_PIN & STEP_IN change interrupt
ISR (PCINT0_vect)
{
  volatile u8 pinb = PINB;
  volatile u8 stepIn = pinb & 0b00000001; //much faster than digitalRead()
  volatile u8 dirIn =  pinb & 0b00000010;
  volatile u8 enIn =   pinb & 0b00000100;

  if (!motorJamming || enIn ) {
    PORTC = pinb; //Forward StepIn,DirIn,EnIn to stepmotor controller
  }

  if (stepIn && lastStepInState==0) { //Rising edge, EN === enabled
    if (dirIn) {
      internal_in_stepCounter--;
    } else {
      internal_in_stepCounter++;
    }
  } 
  lastStepInState = stepIn;
}

volatile const u8 phase_array[] = {
    0b00110000,
    0b00010000,
    0b00000000,
    0b00100000,
};
volatile char previous_phase_index = 0xff;

//Handle quadrature signals
ISR (PCINT2_vect)
{
  volatile u8 pind = PIND & 0b00110000;
  volatile char current_phase_index;
  
  //Search phase index
  for(current_phase_index=0;pind!=phase_array[current_phase_index];current_phase_index++);

  if(((previous_phase_index+1)&0b11)==current_phase_index) {
    #ifdef FLIP_ENCODER
      internal_encoder_position--;
    #else
      internal_encoder_position++;
    #endif
  } else if(((previous_phase_index-1)&0b11)==current_phase_index) {
    #ifdef FLIP_ENCODER
      internal_encoder_position++;
    #else
      internal_encoder_position--;
    #endif
  } else {
    if(previous_phase_index!=0xff) {
      #if ENABLE_PRINTS>0
      Serial.print("Lost phase synchronization!!! ");
      Serial.print((int)current_phase_index);
      Serial.print(",");
      Serial.println((int)previous_phase_index);
      #endif
    }
  }

  public_encoder_position = internal_encoder_position;
  public_in_stepCounter = internal_in_stepCounter;

  previous_phase_index = current_phase_index;
}


float calculateError() {
  long long int a;
  long long int i;

  cli();
  a = public_encoder_position;
  i = public_in_stepCounter;
  sei();

  long long int converted_motor_position = ((i * ((long long int)ENCODER_STATE_CHANGE_PER_REV*100) / (long long int)NEMA_MOTOR_PPR));
  long long int ret = a*100 - converted_motor_position;
 
  #if ENABLE_PRINTS>50 and ENABLE_PRINTS<99
    int converted = ((ENCODER_PPR ) * i) / NEMA_MOTOR_PPR;
    Serial.print((int)a);Serial.print(" ");
    Serial.print((int)i);Serial.print(" ");
    Serial.print(converted);Serial.print(" ");
    Serial.print(ret);Serial.println();
  #endif
    
  return (float)ret/100;
}

int checkErrorDirection() {
  static bool errorFound = false;
  int m = calculateError();

  if (m < -STEP_ERROR_MAX || m > STEP_ERROR_MAX) {
    errorFound = true;
  }

  if (m >= -STEP_ERROR_MIN && m <= STEP_ERROR_MIN) {
    errorFound = false;
  }

  if (errorFound) {
    if (m < 0) {
      return 2;
    } else {
      return 1;
    }
  }
  return 0; //OK
}

#if ENABLE_PRINTS>99
void printDebugInfoToSerialPlotter() {
  static Millis t = Millis(100);
  if (t.check()) {
    t.reset();
    Serial.println(calculateError());
  }
}
#endif

void loop() {
  runMotor();
  #if ENABLE_PRINTS>99
    printDebugInfoToSerialPlotter();
  #endif
}
