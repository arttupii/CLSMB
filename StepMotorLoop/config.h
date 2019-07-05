#ifndef __CONFIG__H_
#define __CONFIG__H_

#define ENCODER_STATE_CHANGE_PER_REV 1024.0 //State change per Revolution (A+B) (AS5040)

#define STEP_MOTOR_REV_PER_SEC 4.0 //revolutions/second. Fixing speed

#define NEMA_MOTOR_FULL_STEPS 200.0
#define NEMA_MOTOR_MICROSTEPS 16.0
#define NEMA_MOTOR_PPR (NEMA_MOTOR_FULL_STEPS * NEMA_MOTOR_MICROSTEPS)

#define STEP_ERROR_MAX 20 //Start position repairing if position deviation is more than this
#define STEP_ERROR_MIN 2 //Stop position repairing if position deviation is less than this

//#define FLIP_ENCODER //enable if rotor position repairing is done wrong direction

#define ENABLE_PRINTS 1

#define STEP_1_PULSE_US 1 //pulse wide

#endif
