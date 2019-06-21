void runMotor() {
  static Micros t;
  static int state = 0;
  static int dir;
  switch (state) {
    case 0: {
        dir = checkStepError();
        if (dir) {
          motorJamming = true;
          errorHappened = true;
          state++;
        } else {
          motorJamming = false;
        }
      }
      break;
    case 1: {
        digitalWrite(MOTOR_STEP, LOW);
        if (dir==1) {
          digitalWrite(MOTOR_DIR, HIGH);
        } else {
          digitalWrite(MOTOR_DIR, LOW);
        }
        t.setTime(STEP_1_PULSE_US);
        digitalWrite(MOTOR_STEP, HIGH);
        state++;
      }
      break;
    case 2: {
        if (t.check()) {
          digitalWrite(MOTOR_STEP, LOW);
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
