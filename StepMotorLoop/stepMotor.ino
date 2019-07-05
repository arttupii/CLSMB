
#define SET_MOTOR_STEP_LOW  PORTC&=0b11111110
#define SET_MOTOR_STEP_HIGH PORTC|=0b00000001

#define SET_MOTOR_DIR_LOW  PORTC&=0b11111101
#define SET_MOTOR_DIR_HIGH PORTC|=0b00000010

#define READ_EN_PIN (PINB & 0b00000100)

#define HOLD_ON_REQ DDRD|=0b10000000 //HOLD_ON is LOW. Use D7 pin like open collector output
#define CANCEL_HOLD_ON_REQ DDRD&=0b01111111 //HOLD_ON is OPEN

#define SET_LED_HIGH PORTB|=0b00100000
#define SET_LED_LOW PORTB&=0b11011111

const u8 defaultDivider = (1000000.0 / (NEMA_MOTOR_PPR*STEP_MOTOR_REV_PER_SEC))/(2*20);

volatile u8 dir;
volatile u8 nextStep = 0;
volatile u8 divider = defaultDivider;

ISR(TIMER2_OVF_vect) {       // timer compare interrupt service routine
  TCNT2 = 245;  //-->20us
  if (divider <= 0) {
    if (dir) {
      if(nextStep) {
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
    divider = defaultDivider;
  }
  divider--;
}

static Millis ledOnTimer = Millis(200);
Micros u;

inline void runMotor() {
  if (READ_EN_PIN) {
    motorJamming=false;
    return;
  }
  dir = checkErrorDirection();
  
  if (dir) {
    motorJamming = true;
    HOLD_ON_REQ;
    SET_LED_HIGH;
    ledOnTimer.reset();
  } else {
    nextStep = 0;
    motorJamming = false;
    divider = defaultDivider;
    CANCEL_HOLD_ON_REQ;
    if (ledOnTimer.check()) {
      SET_LED_LOW;
    }
  }
}
