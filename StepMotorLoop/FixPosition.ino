
#define SET_MOTOR_STEP_LOW  PORTC&=0b11111110
#define SET_MOTOR_STEP_HIGH PORTC|=0b00000001

#define SET_MOTOR_DIR_LOW  PORTC&=0b11111101
#define SET_MOTOR_DIR_HIGH PORTC|=0b00000010

#define READ_EN_PIN (PINB & 0b00000100)

#define HOLD_ON_REQ DDRD|=0b10000000 //HOLD_ON is LOW. Use D7 pin like open collector output
#define CANCEL_HOLD_ON_REQ DDRD&=0b01111111 //HOLD_ON is OPEN

#define SET_LED_HIGH PORTB|=0b00100000
#define SET_LED_LOW PORTB&=0b11011111

#ifdef LOAD_POSITION_CONTROL_MODE

volatile u8 nextStep = 0;
volatile char dir;
volatile long stepErr;
volatile long lastOkPosition = 0;
ISR(TIMER1_COMPA_vect) {       // timer compare interrupt service routine
  TCNT1 = 0;

  //DeviationCounter start

  if (internal_encoder_position < internal_in_stepCounter) {
    dir = -1;
  }
  else if (internal_encoder_position > internal_in_stepCounter) {
    dir = 1;
  }
  stepErr = internal_encoder_position - internal_in_stepCounter;

  if (stepErr >= -STEP_ERROR_MIN && stepErr <= STEP_ERROR_MIN)
  {
    lastOkPosition = internal_encoder_position;
  }

  volatile long s = internal_encoder_position - lastOkPosition;
  if (s > -STEP_ERROR_MAX && s < STEP_ERROR_MAX) {
    dir = 0;
  }

  //DeviationCounter end

  //FixPosition start
  if (dir != 0) {
    if (nextStep) {
      if (dir == 1) {
        SET_MOTOR_DIR_HIGH;
      } else {
        SET_MOTOR_DIR_LOW;
      }
      SET_MOTOR_STEP_HIGH;
      nextStep = 0;
    } else {
      SET_MOTOR_STEP_LOW;
      nextStep = 1;
    }
  }
  //FixPosition end
}

inline void runMotor() {
  //Dummy function...
  //Serial.print(internal_encoder_position-internal_in_stepCounter); Serial.print(" ");
  Serial.print((int)dir); Serial.print("\n");
  if (stepErr < -50 || stepErr > 50)
  {
    SET_LED_HIGH;
  } else {
    SET_LED_LOW;
  }
}
#endif


#ifdef STEP_LOSS_COMPENSATION_MODE
volatile u8 dir;
volatile u8 nextStep = 0;
ISR(TIMER1_COMPA_vect) {       // timer compare interrupt service routine
  TCNT1 = 0;
  if (dir) {
    if (nextStep) {
      if (dir == 1) {
        SET_MOTOR_DIR_HIGH;
      } else {
        SET_MOTOR_DIR_LOW;
      }
      SET_MOTOR_STEP_HIGH;
      nextStep = 0;
    } else {
      SET_MOTOR_STEP_LOW;
      nextStep = 1;
    }
  }
}

unsigned long led_millis = 0;

inline void runMotor() {
  u8 dir_e = checkErrorDirection();
  if (dir_e) {
    motorJamming = true;
    HOLD_ON_REQ;
    SET_LED_HIGH;
    led_millis = millis();
  } else {
    nextStep = 0;
    motorJamming = false;
    CANCEL_HOLD_ON_REQ;
    if ((millis() - led_millis) > 200) {
      SET_LED_LOW;
    }
  }
  if (!READ_EN_PIN) {
    dir = dir_e;
  } else {
    dir = 0;
  }
}
#endif
