
#define SET_MOTOR_STEP_LOW  PORTC&=0b11111110
#define SET_MOTOR_STEP_HIGH PORTC|=0b00000001

#define SET_MOTOR_DIR_LOW  PORTC&=0b11111101
#define SET_MOTOR_DIR_HIGH PORTC|=0b00000010

#define READ_EN_PIN (PINB & 0b00000100)

#define HOLD_ON_REQ DDRD|=0b10000000 //HOLD_ON is LOW. Use D7 pin like open collector output
#define CANCEL_HOLD_ON_REQ DDRD&=0b01111111 //HOLD_ON is OPEN

#define SET_LED_HIGH PORTB|=0b00100000
#define SET_LED_LOW PORTB&=0b11011111

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
  if (READ_EN_PIN) {
    motorJamming = false;
    return;
  }
  dir = checkErrorDirection();

  if (dir) {
    motorJamming = true;
    HOLD_ON_REQ;
    SET_LED_HIGH;
    led_millis = millis();
  } else {
    nextStep = 0;
    motorJamming = false;
    CANCEL_HOLD_ON_REQ;
    if ((millis()-led_millis)>200) {
      SET_LED_LOW;
    }
  }
}
