#include <avr/interrupt.h>
#include <Micros.h>
#include <Millis.h>
#define MOTOR_EN A3
#define MOTOR_MS1 A0
#define MOTOR_MS2 A1
#define MOTOR_MS3 A2
#define MOTOR_STEP 13
#define MOTOR_DIR 12

#define AS5040_CSN 10
#define AS5040_CLK A5
#define AS5040_D0 11
#define AS5040_PROG A4
#define AS5040_INDEX_W 12
#define AS5040_B_DIR_V 3 //DIR
#define AS5040_A_DIR_U 2 //LSB
#define AS5040_MAGINC 7
#define AS5040_MAGDEC 8

#define IN_STEP 4
#define IN_DIR 5
#define IN_EN 6

#define LED 7


#define STEP_0_PULSE_US 500
#define STEP_1_PULSE_US 500



void setup() {
  // put your setup code here, to run once:
  pinMode(MOTOR_EN, OUTPUT);
  pinMode(MOTOR_STEP, OUTPUT);
  pinMode(MOTOR_DIR, OUTPUT);
  pinMode(MOTOR_MS1, OUTPUT);
  pinMode(MOTOR_MS2, OUTPUT);
  pinMode(MOTOR_MS2, OUTPUT);

  pinMode(LED, OUTPUT);

  pinMode(IN_STEP, INPUT);
  pinMode(IN_DIR, INPUT);
  pinMode(IN_EN, INPUT);

  //Resolution 16 step
  digitalWrite(MOTOR_MS1, HIGH);
  digitalWrite(MOTOR_MS2, HIGH);
  digitalWrite(MOTOR_MS3, HIGH);

  attachInterrupt(digitalPinToInterrupt(AS5040_A_DIR_U), interrupt_AS5040_A_DIR_U, FALLING );
  attachInterrupt(digitalPinToInterrupt(AS5040_A_DIR_U), interrupt_input_IN_STEP, RISING );

  cli();
  PCICR |= 0b00000100; // Enables Ports B and C Pin Change Interrupts
  //PCMSK0 |= 0b00000001; // PCINT0
  //PCMSK1 |= 0b00001000; // PCINT11
  PCMSK2 |= 0b00000111; // PCINT11
  sei();

  //512 pulses per revolution
  digitalWrite(AS5040_CLK, LOW);
  digitalWrite(AS5040_CSN, HIGH);
  digitalWrite(AS5040_PROG, HIGH);
  send_AS5040_bit(1); //Clockwise operation
  send_AS5040_bit(0); // No programmed zero position Z9
  send_AS5040_bit(0); // No programmed zero position Z8
  send_AS5040_bit(0); // No programmed zero position Z7
  send_AS5040_bit(0); // No programmed zero position Z6
  send_AS5040_bit(0); // No programmed zero position Z5
  send_AS5040_bit(0); // No programmed zero position Z4
  send_AS5040_bit(0); // No programmed zero position Z3
  send_AS5040_bit(0); // No programmed zero position Z2
  send_AS5040_bit(0); // No programmed zero position Z1
  send_AS5040_bit(0); // No programmed zero position Z0
  send_AS5040_bit(0); //  Index bit width
  send_AS5040_bit(0); //  Incremental resolution Div1
  send_AS5040_bit(0); //  Incremental resolution Div0
  send_AS5040_bit(1); //  = Incremental mode = quadrature  MD1
  send_AS5040_bit(0); //  = Incremental mode = quadrature  MD0
  digitalWrite(AS5040_CSN, LOW);
}

void send_AS5040_bit(int v) {
  digitalWrite(AS5040_CLK, LOW);
  digitalWrite(AS5040_PROG, v ? HIGH : LOW);
  delay(1);
  digitalWrite(AS5040_CLK, HIGH);
  delay(1);
  digitalWrite(AS5040_CLK, LOW);
}

long int AS5040_stepCounter = 0;
long int in_stepCounter = 0;

void interrupt_AS5040_A_DIR_U() {
  if (digitalRead(AS5040_B_DIR_V)) {
    AS5040_stepCounter++;
  } else {
    AS5040_stepCounter--;
  }
}
void interrupt_input_IN_STEP() {
  if (digitalRead(IN_DIR)) {
    in_stepCounter++;
  } else {
    in_stepCounter--;
  }
}


bool motorJamming = false;
bool errorHappened = false;

ISR(PCINT2_vect)
{
  if (!motorJamming) {
    if (digitalRead(IN_STEP)) {
      digitalWrite(MOTOR_STEP, HIGH);
    } else {
      digitalWrite(MOTOR_STEP, LOW);
    }
    if (digitalRead(IN_DIR)) {
      digitalWrite(MOTOR_DIR, HIGH);
    } else {
      digitalWrite(MOTOR_DIR, LOW);
    }
  }
  if (digitalRead(IN_EN)) {
    digitalWrite(MOTOR_EN, HIGH);
  } else {
    digitalWrite(MOTOR_EN, LOW);
  }
}

bool stateMachineLed() {
  static Micros t;
  static int state = 0;

  if (errorHappened && state<10) {
    state = 10;
  }
  if (motorJamming) {
    state = 20;
  }

  switch (state) {
    case 0:
      digitalWrite(LED, LOW);
      state++;
      break;
    case 1:
      break;
    case 10:
      digitalWrite(LED, HIGH);
      t.setTime(100);
      state++;
      break;
    case 11:
      if (t.check()) {
        digitalWrite(LED, LOW);
        t.setTime(500);
        state++;
      }
    case 12:
      if (t.check()) {
        state = 0;
      }
      break;
    case 20:
        digitalWrite(LED, HIGH);
        t.setTime(1000);
        state++;
        break;
    case 21:
      if (t.check()) {
        digitalWrite(LED, LOW);
        state = 0;
      }
      break;
  }
}

bool checkStepError() {
  return false;
}
bool getMotorDirection() {
  return true;
}

void stateMachine() {
  static Micros t;
  static int state = 0;
  switch (state) {
    case 0: {
        if (checkStepError()) {
          motorJamming = true;
          errorHappened = true;
          state++;
        } else {
          motorJamming = false;
        }
      }
      break;
    case 1: {
        digitalWrite(LED, HIGH);

        digitalWrite(MOTOR_STEP, LOW);
        if (getMotorDirection()) {
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

void printDebugInfo() {
  static Millis t = Millis(3000);
  if (t.check()) {
    t.reset();

  }
}

void loop() {
  stateMachine();
  stateMachineLed();
}
