
#define SET_MOTOR_STEP_LOW  PORTC&=0b11111110
#define SET_MOTOR_STEP_HIGH PORTC|=0b00000001

#define SET_MOTOR_DIR_LOW  PORTC&=0b11111101
#define SET_MOTOR_DIR_HIGH PORTC|=0x00000010

#define READ_EN_PIN (PINB & 0b00000100)

void runMotor() {
  static Micros t;
  static int state = 0;
  static int dir;

  if (READ_EN_PIN) {
    return;
  }

  switch (state) {
    case 0: { //Check errors
        dir = checkErrorDirection();
        if (dir) {
          motorJamming = true;
          errorHappened = true;
          state++;
        } else {
          motorJamming = false;
          break;
        }
      }
    case 1: {
        SET_MOTOR_STEP_LOW;
        if (dir == 1) {
          SET_MOTOR_DIR_HIGH;
        } else {
          SET_MOTOR_DIR_LOW;
        }
        t.setTime(STEP_1_PULSE_US);

        SET_MOTOR_STEP_HIGH;
        state++;
      }
    case 2: {
        if (t.check()) {
          SET_MOTOR_STEP_LOW;
          t.setTime(STEP_0_PULSE_US);
          state++;
        }
        break;
      }
    case 3: {
        if (t.check()) {
          state = 0;
        }
      }
      break;
    default:
      state = 0;
      break;
  }
}
