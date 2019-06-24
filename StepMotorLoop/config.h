#ifndef __CONFIG__H_
#define __CONFIG__H_

//#define QUADRATURE_MODE //Quadrature signal support

#ifdef QUADRATURE_MODE
  #define ENCODER_A_PIN 4 //PCINT20 pin change interrupt. If you change this --> update PCICR&PCMSK2 registers
  #define ENCODER_B_PIN 5 //PCINT21 pin change interrupt. 

  #define MOTOR_EN A3
  #define MOTOR_MS1 A0
  #define MOTOR_MS2 A1
  #define MOTOR_MS3 A2
  #define MOTOR_STEP 13
  #define MOTOR_DIR 12
  
  #define IN_STEP 3  //Connected to INT0 
  #define IN_DIR 6   
  #define IN_EN 7
  #define LED 7
  #define ENCODER_PPR 1600 //Pulses per Revolution
#else
  #define STEP_DIR_MODE
  //#define PROGRAM_AS5040
  #define AS5040_CSN 10
  #define AS5040_CLK A5
  #define AS5040_D0 11
  #define AS5040_PROG A4
  #define AS5040_B_DIR_V 8 //DIR
  #define AS5040_A_DIR_U 2 //LSB

  #define MOTOR_EN A3
  #define MOTOR_MS1 A0
  #define MOTOR_MS2 A1
  #define MOTOR_MS3 A2
  #define MOTOR_STEP 13
  #define MOTOR_DIR 12
  
  #define IN_STEP 3
  #define IN_DIR 5
  #define IN_EN 6
  #define LED 8
  #define ENCODER_PPR 512 //Pulses per Revolution
#endif

#define STEP_0_PULSE_US 50
#define STEP_1_PULSE_US 50

#define MOTOR_PPR (400*16) //Pulses per Revolution

#endif
