#ifndef __CONFIG__H_
#define __CONFIG__H_

#define ENCODER_PPR 1024 //Pulses per Revolution (AS5040)
//#define ENCODER_PPR 400 //Pulses per Revolution (HN3806-AB -400N)

#define STEP_MOTOR_RPM 1000 //rotations per minute. Fixing speed

#define NEMA_MOTOR_FULL_STEPS 200
#define NEMA_MOTOR_MICROSTEPS 16
#define NEMA_MOTOR_PPR (NEMA_MOTOR_FULL_STEPS * NEMA_MOTOR_MICROSTEPS)

#define STEP_ERROR_MAX 50 //Start position repairing if position deviation is more than this
#define STEP_ERROR_MIN 10 //Stop position repairing if position deviation is less than this

//#define FLIP_ENCODER //enable if rotor position repairing is done wrong direction

#define ENABLE_PRINTS 0

#define STEP_1_PULSE_US 10 //pulse wide
#define STEP_0_PULSE_US (NEMA_MOTOR_PPR/60/STEP_1_PULSE_US)

#endif
