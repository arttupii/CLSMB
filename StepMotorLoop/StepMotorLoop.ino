
#include <avr/interrupt.h>
#include <Micros.h>
#include <Millis.h>
#include "config.h"


#define CSpin   AS5040_CSN
#define CLKpin  AS5040_CLK
#define DOpin   AS5040_D0


void setup() {
  Serial.begin(115200);
  cli();
  // put your setup code here, to run once:
  pinMode(MOTOR_EN, OUTPUT);
  pinMode(MOTOR_STEP, OUTPUT);
  pinMode(MOTOR_DIR, OUTPUT);

  pinMode(LED, OUTPUT);

  pinMode(IN_STEP, INPUT);
  pinMode(IN_DIR, INPUT);
  pinMode(IN_EN, INPUT);



#ifdef STEP_DIR_MODE
    pinMode(MOTOR_MS1, OUTPUT);
    pinMode(MOTOR_MS2, OUTPUT);
    pinMode(MOTOR_MS2, OUTPUT);
    
    //Resolution 16 step    
    digitalWrite(MOTOR_MS1, HIGH);
    digitalWrite(MOTOR_MS2, HIGH);
    digitalWrite(MOTOR_MS3, HIGH);
    
    digitalWrite(AS5040_D0, HIGH);
    digitalWrite(AS5040_PROG, LOW);
    digitalWrite(AS5040_B_DIR_V, HIGH);
    digitalWrite(AS5040_A_DIR_U, HIGH);
  
    pinMode(AS5040_D0, INPUT);
    pinMode(AS5040_PROG, OUTPUT);
    pinMode(AS5040_B_DIR_V, INPUT);
    pinMode(AS5040_A_DIR_U, INPUT);
  
  
    pinMode(AS5040_CSN, OUTPUT);
    pinMode(AS5040_CLK, OUTPUT);
  
  
    digitalWrite(AS5040_CLK, LOW);
    digitalWrite(AS5040_CSN, LOW);
  
    //*************************************************
    //readAS5045();
    //***********************************************
  
  
    delay(1);
  
    attachInterrupt(digitalPinToInterrupt(AS5040_A_DIR_U), interrupt_AS5040_A_DIR_U, RISING );
    
  
  #ifdef PROGRAM_AS5040
    //512 pulses per revolution
    digitalWrite(AS5040_CLK, LOW);
    digitalWrite(AS5040_CSN, LOW);
    digitalWrite(AS5040_PROG, LOW);
    delayMicroseconds (5) ;
  
    digitalWrite(AS5040_PROG, HIGH);
    delayMicroseconds (5);
    digitalWrite(AS5040_CSN, HIGH);
    delayMicroseconds (5);
      
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
  delayMicroseconds (5) ;
    digitalWrite(AS5040_PROG, LOW);
    delayMicroseconds (5) ;
    digitalWrite(AS5040_CSN, LOW);
    delayMicroseconds (5) ;
    digitalWrite(AS5040_CLK, LOW);
  #endif
    digitalWrite(AS5040_CSN, LOW);
    digitalWrite(MOTOR_EN, LOW);
    sei();
#endif

  attachInterrupt(digitalPinToInterrupt(IN_STEP), interrupt_input_IN_STEP, CHANGE );

  #ifdef QUADRATURE_MODE
    pinMode(ENCODER_A_PIN, INPUT);
    pinMode(ENCODER_B_PIN, INPUT);
    digitalWrite(ENCODER_A_PIN, HIGH); //PullUp resistor
    digitalWrite(ENCODER_B_PIN, HIGH); //PullUp presistor
       
    cli();//Disable interrupts
    //See https://sites.google.com/site/qeewiki/books/avr-guide/external-interrupts-on-the-atmega328
    PCICR==0b00000100; //Enable PCIE2
    PCMSK2=0b00110000; //Pin Change Mask Register. Enable Pin change interrupt for PCINT20&PCINT21 
    sei();//Enable interrupts
  #endif
}


#ifdef STEP_DIR_MODE
void send_AS5040_bit(int v) {
  digitalWrite(AS5040_CLK, LOW);
  digitalWrite(AS5040_PROG, v ? HIGH : LOW);
  delayMicroseconds (5) ;
  digitalWrite(AS5040_CLK, HIGH);
  delayMicroseconds (5) ;
  digitalWrite(AS5040_CLK, LOW);
  delayMicroseconds (5) ;
}
#endif

long int volatile encoder_position = 0;
long int volatile in_stepCounter = 0;

long int volatile max=0,min=0;
long int volatile max1=0,min1=0;

#ifndef QUADRATURE_MODE
void interrupt_AS5040_A_DIR_U() {
  if (digitalRead(AS5040_B_DIR_V)) {
    encoder_position--;
  } else {
    encoder_position++;
  }

    if(max<encoder_position) max=encoder_position;
  if(min>encoder_position) min=encoder_position;
  
}
#endif

bool motorJamming = false;
bool errorHappened = false;

int errSteps;
void interrupt_input_IN_STEP() {
  int stepIn = digitalRead(IN_STEP);
  int dirIn= digitalRead(IN_DIR);
  if(stepIn) {
    if (dirIn) {
      in_stepCounter++;
    } else {
      in_stepCounter--;
    }
    if(max1<in_stepCounter) max1=in_stepCounter;
    if(min1>in_stepCounter) min1=in_stepCounter;
  }
  if (!motorJamming) {
     digitalWrite(MOTOR_DIR, dirIn);
     digitalWrite(MOTOR_STEP, stepIn);
  }
}

#ifdef QUADRATURE_MODE
//Handle quafrature signals
ISR (PCINT2_vect)
{
  //Copied from https://reprap.org/wiki/Magnetic_Rotary_Encoder_v1.0
  
  // found a low-to-high on channel A
  if (digitalRead(ENCODER_A_PIN) == HIGH)
  {   
    // check channel B to see which way
    if (digitalRead(ENCODER_B_PIN) == LOW)
        encoder_position++;
    else
        encoder_position--;
  }
  // found a high-to-low on channel A
  else                                        
  {
    // check channel B to see which way
    if (digitalRead(ENCODER_B_PIN) == LOW)
        encoder_position--;
    else
        encoder_position++;
  }
}
#endif

int calculateError() {
  cli();
  long int volatile a = encoder_position;
  long int volatile i = in_stepCounter;
  sei();
  
  return (a)-((ENCODER_PPR )* i) / MOTOR_PPR;
}

int checkStepError() {
  static bool errorFound = false;
  int m = calculateError();
  
  if (m < -50 || m > 50) {
    errorFound = true;
  }
  
  if (m >= -5 && m <= 5) {
    errorFound = false;
  }

  if(errorFound) {
    if (m<0) {
      return 1;
    } else {
      return 2;
    }
  }
  return 0;
}

void printDebugInfo() {
  static Millis t = Millis(100);
  if (t.check()) {
    t.reset();
    Serial.println(calculateError());
  }
}

void loop() {
  //digitalWrite(MOTOR_EN, digitalRead(IN_EN));
  runMotor();
  runLed();
  runMotor();
  printDebugInfo();
  runMotor();
}
