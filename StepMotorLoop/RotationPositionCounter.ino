#ifdef X4_ENCODING
volatile u8 lastEncoder = 0;
volatile const u8 x4_phase_array[] = {
  0b11,
  0b01,
  0b00,
  0b10,
};

#endif

//Types of quadrature encoding
//https://www.linearmotiontips.com/what-is-quadrature-encoding/

#ifdef FLIP_ENCODER
#define INCREASE_COUNTER internal_encoder_position--;
#define DECREASE_COUNTER internal_encoder_position++;
#else
#define INCREASE_COUNTER internal_encoder_position++;
#define DECREASE_COUNTER internal_encoder_position--;
#endif


void ICACHE_RAM_ATTR handleInterruptSignalA()
{ //76543210
  volatile u8 encoder = (digitalRead(PIN_IN_B)<<1) | digitalRead(PIN_IN_A);
#ifdef X1_ENCODING
  if (encoder & 0b10) {
    INCREASE_COUNTER;
  } else {
    DECREASE_COUNTER
  }
#endif
#ifdef X2_ENCODING
  if (encoder == 0b10 || encoder == 0b01) {
    DECREASE_COUNTER
  }
  if (encoder == 0b11 || encoder == 0b00) {
    INCREASE_COUNTER;
  }
#endif
#ifdef X4_ENCODING
  #if 1
    volatile u8 sum = (lastEncoder<<2) | encoder;
    lastEncoder = encoder;
    if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) {INCREASE_COUNTER;}
    else if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) {DECREASE_COUNTER;}
    else {
  #if ENABLE_PRINTS>0
     Serial.print("Lost phase synchronization!!!\n");
  #endif
    }
  #else
    if (x4_phase_array[(lastEncoder + 1) & 0b11] == encoder) {
      INCREASE_COUNTER
      lastEncoder = (lastEncoder + 1) & 0b11;
    } else if (x4_phase_array[(lastEncoder - 1) & 0b11] == encoder) {
      DECREASE_COUNTER
      lastEncoder = (lastEncoder - 1) & 0b11;
    } else {
  #if ENABLE_PRINTS>0
     Serial.print("Lost phase synchronization!!!\n");
  #endif
  
  
    }
  #endif

  
#endif
}
