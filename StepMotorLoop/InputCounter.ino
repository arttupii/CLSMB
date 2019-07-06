volatile u8 lastStepInState = 0;

//Handle EN_PIN & STEP_IN change interrupt
ISR (PCINT0_vect)
{
  volatile u8 pinb = PINB;
  volatile u8 stepIn = pinb & 0b00000001; //much faster than digitalRead()
  volatile u8 dirIn =  pinb & 0b00000010;
  volatile u8 enIn =   pinb & 0b00000100;

  if (!motorJamming && enIn==0 ) {
   // PORTC = pinb; //Forward StepIn,DirIn,EnIn to stepmotor controlle
   #ifdef LOAD_POSITION_CONTROL
      if(enIn) {
        PORTC|=0b00000100;
      } else {
        PORTC&=0b11111011;
      }
   #else
      PORTC = pinb;
   #endif
  }

  if (stepIn && lastStepInState == 0 && enIn==0 ) { //Rising edge, EN === enabled
    if (dirIn) {
      internal_in_stepCounter--;
    } else {
      internal_in_stepCounter++;
    }
  }
  lastStepInState = stepIn;
}
