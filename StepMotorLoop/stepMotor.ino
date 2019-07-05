
#define SET_MOTOR_STEP_LOW  PORTC&=0b11111110
#define SET_MOTOR_STEP_HIGH PORTC|=0b00000001

#define SET_MOTOR_DIR_LOW  PORTC&=0b11111101
#define SET_MOTOR_DIR_HIGH PORTC|=0b00000010

#define READ_EN_PIN (PINB & 0b00000100)

#define HOLD_ON_REQ DDRD|=0b10000000 //HOLD_ON is LOW. Use D7 pin like open collector output
#define CANCEL_HOLD_ON_REQ DDRD&=0b01111111 //HOLD_ON is OPEN

#define SET_LED_HIGH PORTB|=0b00100000
#define SET_LED_LOW PORTB&=0b11011111

const long int step_0_pulse_wide = (1000000.0/(NEMA_MOTOR_PPR*STEP_MOTOR_REV_PER_SEC))-STEP_1_PULSE_US - 50.0; //checErrorDirection() takes 50us --> -50us

static Millis ledOnTimer = Millis(200);
Micros u;
u8 i;
inline void runMotor() {
  static int dir;

  if (READ_EN_PIN) {
    return;
  }

  if(i>200 || motorJamming&&(i>2)) { //Sceduling...
    dir = checkErrorDirection();
    i=0;
  }
    i++;

  if (dir) {
      motorJamming = true;
      HOLD_ON_REQ;
      SET_LED_HIGH;
      ledOnTimer.reset();
    } else {
      motorJamming = false;
      CANCEL_HOLD_ON_REQ;
      if (ledOnTimer.check()) {
         SET_LED_LOW;
      }
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

  if(step_0_pulse_wide>0) {
    delayMicroseconds(step_0_pulse_wide); 
  }
}
