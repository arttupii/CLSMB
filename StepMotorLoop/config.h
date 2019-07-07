#ifndef __CONFIG__H_
#define __CONFIG__H_

#define ENCODER_STATE_CHANGE_PER_REV 1024.0 //State change per Revolution (A+B) (AS5040)


#define NEMA_MOTOR_FULL_STEPS 200.0
#define NEMA_MOTOR_MICROSTEPS 16.0
#define NEMA_MOTOR_PPR (NEMA_MOTOR_FULL_STEPS * NEMA_MOTOR_MICROSTEPS)


//#define FLIP_ENCODER //enable if rotor position repairing is done wrong direction

//Select loop back mode
//https://www.linearmotiontips.com/how-does-closed-loop-stepper-control-work/
#define STEP_LOSS_COMPENSATION_MODE
//#define LOAD_POSITION_CONTROL_MODE //Hox! This mode doesn't work yeat


#define ENABLE_PRINTS 1

#define STEP_1_PULSE_US 1 //pulse wide


#ifdef STEP_LOSS_COMPENSATION_MODE
  #define STEP_ERROR_MAX 20 //Start position repairing if position deviation is more than this
  #define STEP_ERROR_MIN 10 //Stop position repairing if position deviation is less than this
  #define STEP_MOTOR_REV_PER_SEC 3.5 //revolutions/second. Fixing speed
#endif

#ifdef LOAD_POSITION_CONTROL_MODE
  #define STEP_MOTOR_REV_PER_SEC 3.0 //revolutions/second. Fixing speed
  #define STEP_ERROR_MAX 3 //Start position repairing if position deviation is more than this
  #define STEP_ERROR_MIN 2 //Stop position repairing if position deviation is less than this
#endif


#ifdef STEP_LOSS_COMPENSATION
  #ifdef LOAD_POSITION_CONTROL
    #error "Selected STEP_LOSS_COMPENSATION & LOAD_POSITION_CONTROL!!"
  #endif
#endif
#endif
