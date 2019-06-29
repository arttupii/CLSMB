
#define SET_MOTOR_STEP_LOW  PORTC&=0b11111110
#define SET_MOTOR_STEP_HIGH PORTC|=0b00000001

#define SET_MOTOR_DIR_LOW  PORTC&=0b11111101
#define SET_MOTOR_DIR_HIGH PORTC|=0b00000010

#define READ_EN_PIN (PINB & 0b00000100)

#define HOLD_ON_REQ DDRD|=0b10000000 //HOLD_ON is LOW. Use D7 pin like open collector output
#define CANCEL_HOLD_ON_REQ DDRD&=0b01111111 //HOLD_ON is OPEN


void runMotor() {
  int dir;

  if (READ_EN_PIN) {
    return;
  }

  dir = checkErrorDirection();
  if (dir) {
    motorJamming = true;
    errorHappened = true;
    HOLD_ON_REQ;
  } else {
    motorJamming = false;
    CANCEL_HOLD_ON_REQ;
    return;
  }
  
  SET_MOTOR_STEP_LOW;
  if (dir == 1) {
    SET_MOTOR_DIR_HIGH;
  } else {
    SET_MOTOR_DIR_LOW;
  }

  SET_MOTOR_STEP_HIGH;
  delayMicroseconds(STEP_1_PULSE_US);
  SET_MOTOR_STEP_LOW;
  delayMicroseconds(STEP_0_PULSE_US);
}
