#ifndef __CONFIG__H_
#define __CONFIG__H_

//#define PROGRAM_AS5040

#define MOTOR_EN A3
#define MOTOR_MS1 A0
#define MOTOR_MS2 A1
#define MOTOR_MS3 A2
#define MOTOR_STEP 13
#define MOTOR_DIR 12

#define AS5040_CSN 10
#define AS5040_CLK A5
#define AS5040_D0 11
#define AS5040_PROG A4
#define AS5040_B_DIR_V 8 //DIR
#define AS5040_A_DIR_U 2 //LSB


#define IN_STEP 3
#define IN_DIR 5
#define IN_EN 6

#define LED 7


#define STEP_0_PULSE_US 50
#define STEP_1_PULSE_US 50



#define AS5040_PPR 512 //Pulses per Revolution
#define MOTOR_PPR (400*16) //Pulses per Revolution


#endif
