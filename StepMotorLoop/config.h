#ifndef __CONFIG__H_
#define __CONFIG__H_

#define ENCODER_PPR 1024 //Pulses per Revolution (AS5040)
//#define ENCODER_PPR 400 //Pulses per Revolution (HN3806-AB -400N)

#define STEP_MOTOR_REV_PER_SEC 15 //revolutions/second. Fixing speed

#define NEMA_MOTOR_FULL_STEPS 200
#define NEMA_MOTOR_MICROSTEPS 16
#define NEMA_MOTOR_PPR (NEMA_MOTOR_FULL_STEPS * NEMA_MOTOR_MICROSTEPS)

#define STEP_ERROR_MAX 50 //Start position repairing if position deviation is more than this
#define STEP_ERROR_MIN 10 //Stop position repairing if position deviation is less than this

//#define FLIP_ENCODER //enable if rotor position repairing is done wrong direction

#define ENABLE_PRINTS 100

#define STEP_1_PULSE_US 8 //pulse wide

#endif
