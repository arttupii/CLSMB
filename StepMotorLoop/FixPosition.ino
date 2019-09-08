  
#define SET_MOTOR_STEP_LOW  digitalWrite(PIN_STEP_OUT, LOW)
#define SET_MOTOR_STEP_HIGH digitalWrite(PIN_STEP_OUT, HIGH)

#define SET_MOTOR_DIR_LOW  digitalWrite(PIN_DIR_OUT, LOW)
#define SET_MOTOR_DIR_HIGH digitalWrite(PIN_DIR_OUT, HIGH)

#define READ_EN_PIN digitalRead(PIN_EN_IN)


#define SET_LED_HIGH digitalWrite(PIN_LED,HIGH)
#define SET_LED_LOW  digitalWrite(PIN_LED,LOW)


volatile u8 dir;
volatile u8 nextStep = 0;

void ICACHE_RAM_ATTR onTimerISR(){       // timer compare interrupt service routine
  if (dir || true) {
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
    SET_LED_HIGH;
    led_millis = millis();
  } else {
  //  nextStep = 0;
    motorJamming = false;
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
