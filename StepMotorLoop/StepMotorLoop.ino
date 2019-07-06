
#include <avr/interrupt.h>
#include "config.h"

volatile long internal_in_stepCounter = 0;
volatile long internal_encoder_position = 0;
volatile bool motorJamming = false;

unsigned short calculateOCR1A(unsigned short preScaler, float us) {
  const float u = 1.0 / (16000000.0 / preScaler);
  float ret = (float(us) / (u * 1000000));
  return ret;
}

void setup() {
#if ENABLE_PRINTS>0
  Serial.begin(230400);
  Serial.println("Starting...");
#endif

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


  // initialize Timer1
  TCCR1A = 0;    // set entire TCCR1A register to 0
  TCCR1B = 0;    // set entire TCCR1A register to 0

  //Enable Compare A Interrupt
  bitSet(TIMSK1, OCIE1A);

  long step_pulse_width_us = (1000000.0 / (NEMA_MOTOR_PPR * STEP_MOTOR_REV_PER_SEC)) / 2;
  if (step_pulse_width_us >= 1048560L) { //1048560L = (1/(16MHz/256))*0xffff    (256=prescaler)
    OCR1A = calculateOCR1A(1024, step_pulse_width_us);
    TCCR1B = 0b101; //prescaler 1024
  } else if (step_pulse_width_us >= 262140L) {
    OCR1A = calculateOCR1A(256, step_pulse_width_us);
    TCCR1B = 0b100; //prescaler 256
  } else if (step_pulse_width_us >= 32767) {
    OCR1A = calculateOCR1A(64, step_pulse_width_us);
    TCCR1B = 0b011; //prescaler 64
  } else if (step_pulse_width_us >= 4095) {
    OCR1A = calculateOCR1A(8, step_pulse_width_us);
    TCCR1B = 0b010; //prescaler 8
  } else {
    OCR1A = calculateOCR1A(1, step_pulse_width_us);
    TCCR1B = 0b001; //prescaler 1
  }

  sei();//Enable interrupts
}

inline void printDebugInfoToSerialPlotter() {
  static unsigned long t = millis();
  if ((millis() - t) > 100) {
    t = millis();
#ifdef LOAD_POSITION_CONTROL_MODE
    Serial.println(calculateErrorLoadPositionMode());
#else
    Serial.println(calculateErrorStepLossCompensationMode());
#endif
  }
}

void loop() {
  runMotor();
#if ENABLE_PRINTS>99
  printDebugInfoToSerialPlotter();
#endif
}
