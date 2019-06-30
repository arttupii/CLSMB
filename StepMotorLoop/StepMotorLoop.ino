
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

long long int volatile encoder_position = 0;
long long int volatile in_stepCounter = 0;

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
      in_stepCounter--;
    } else {
      in_stepCounter++;
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
  
  /*for(int i=0;i<4;i++) {
    if(pind==phase_array[i]) {
      current_phase_index=i;
      break;
    }
  }*/ //Search phase index
  for(current_phase_index=0;pind!=phase_array[current_phase_index];current_phase_index++);

  if(((previous_phase_index+1)&0b11)==current_phase_index) {
    #ifdef FLIP_ENCODER
      encoder_position--;
    #else
      encoder_position++;
    #endif
  } else if(((previous_phase_index-1)&0b11)==current_phase_index) {
    #ifdef FLIP_ENCODER
      encoder_position++;
    #else
      encoder_position--;
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

  previous_phase_index = current_phase_index;
}


int calculateError() {
  long long int volatile a;
  long long int volatile i;

  cli();
  a = encoder_position;
  i = in_stepCounter;
  sei();

  int ret = (a) - ((ENCODER_STATE_CHANGE_PER_REV ) * i) / NEMA_MOTOR_PPR;

  #if ENABLE_PRINTS>50 and ENABLE_PRINTS<99
    int converted = ((ENCODER_PPR ) * i) / NEMA_MOTOR_PPR;
    Serial.print((int)a);Serial.print(" ");
    Serial.print((int)i);Serial.print(" ");
    Serial.print(converted);Serial.print(" ");
    Serial.print(ret);Serial.println();
  #endif
    
  return ret;
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
  runLed();
  runMotor();
  #if ENABLE_PRINTS>99
    printDebugInfoToSerialPlotter();
  #endif
}
