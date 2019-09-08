
//Handle EN_PIN & STEP_IN change interrupt

void ICACHE_RAM_ATTR handleInterruptStepIn()
{
  volatile u8 stepIn = digitalRead(PIN_STEP_IN);
  volatile u8 dirIn =  digitalRead(PIN_DIR_IN);
  volatile u8 enIn =   digitalRead(PIN_EN_IN);

  if (!motorJamming && enIn==0 ) {
     digitalWrite(PIN_DIR_OUT, dirIn);  
     digitalWrite(PIN_STEP_OUT, HIGH);
     digitalWrite(PIN_STEP_OUT, LOW);
  }
  if (enIn==0 ) {
    if (dirIn) {
      internal_in_stepCounter--;
    } else {
      internal_in_stepCounter++;
    }
  }
}

void ICACHE_RAM_ATTR handleInterruptEnIn()
{
    digitalWrite(PIN_EN_OUT, digitalRead(PIN_EN_IN));
}
