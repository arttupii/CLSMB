volatile u8 lastStepInState = 0;

//Handle EN_PIN & STEP_IN change interrupt
ICACHE_RAM_ATTR void handleInterruptStepIn()
{
  volatile u8 stepIn = digitalRead(PIN_STEP_IN);
  volatile u8 dirIn =  digitalRead(PIN_DIR_IN);
  volatile u8 enIn =   digitalRead(PIN_EN_IN);

  if (!motorJamming && enIn==0 ) {
     digitalWrite(PIN_EN_OUT, enIn);
     digitalWrite(PIN_DIR_OUT, dirIn);  
     digitalWrite(PIN_STEP_OUT, stepIn);
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
