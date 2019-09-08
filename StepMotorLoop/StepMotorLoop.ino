
#include <ESP8266WiFi.h>
#include <Ticker.h>

#include "config.h"

volatile long internal_in_stepCounter = 0;
volatile long internal_encoder_position = 0;
volatile bool motorJamming = false;

void ICACHE_RAM_ATTR onTimerISR();

void setup() {
#if ENABLE_PRINTS>0
  Serial.begin(230400);
  Serial.println("Starting...");
#endif
  cli();
  pinMode(PIN_IN_A, INPUT_PULLUP);
  pinMode(PIN_IN_B, INPUT_PULLUP);

  pinMode(PIN_STEP_IN, INPUT_PULLUP);
  pinMode(PIN_DIR_IN, INPUT_PULLUP);
  pinMode(PIN_EN_IN, INPUT_PULLUP);

  pinMode(PIN_STEP_OUT, OUTPUT);
  pinMode(PIN_EN_OUT, OUTPUT);
  pinMode(PIN_DIR_OUT, OUTPUT);

  pinMode(PIN_LED, OUTPUT);
  

 attachInterrupt(digitalPinToInterrupt(PIN_STEP_IN), handleInterruptStepIn, RISING);
 attachInterrupt(digitalPinToInterrupt(PIN_EN_IN), handleInterruptStepIn, CHANGE);

  #ifdef X1_ENCODING
  //attachInterrupt(digitalPinToInterrupt(PIN_IN_A), handleInterruptSignalA, RISING);
  #endif
  #ifdef X2_ENCODING
  attachInterrupt(digitalPinToInterrupt(PIN_IN_A), handleInterruptSignalA, CHANGE);
  #endif
  #ifdef X4_ENCODING
  attachInterrupt(digitalPinToInterrupt(PIN_IN_A), handleInterruptSignalA, RISING);
  attachInterrupt(digitalPinToInterrupt(PIN_IN_B), handleInterruptSignalB, RISING);
  #endif 

  long step_pulse_width_us = ((1000000.0 / (NEMA_MOTOR_PPR * STEP_MOTOR_REV_PER_SEC)) / 2)  * 160;
  timer1_attachInterrupt(onTimerISR);
  timer1_enable(TIM_DIV1, TIM_EDGE, TIM_LOOP);
  timer1_write(step_pulse_width_us); //120000 us
  sei();//Enable interrupts
}

inline void printDebugInfoToSerialPlotter() {
  static unsigned long t = millis();
  if ((millis() - t) > 100) {
    t = millis();
#ifdef STEP_LOSS_COMPENSATION_MODE
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
