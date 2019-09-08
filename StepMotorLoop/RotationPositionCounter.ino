#ifdef X4_ENCODING
volatile const u8 x4_phase_array[] = {
  0b11,
  0b01,
  0b00,
  0b10,
};
volatile u8 x4_phase_index = 0;
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
  volatile u8 pind = (digitalRead(PIN_IN_B)?2:0) + (digitalRead(PIN_IN_A)?1:0);
#ifdef X1_ENCODING
  if (pind & 0b10) {
    INCREASE_COUNTER;
  } else {
    DECREASE_COUNTER
  }
#endif
#ifdef X2_ENCODING
  if (pind == 0b10 || pind == 0b01) {
    DECREASE_COUNTER
  }
  if (pind == 0b11 || pind == 0b00) {
    INCREASE_COUNTER;
  }
#endif
#ifdef X4_ENCODING
  if (x4_phase_array[(x4_phase_index + 1) & 0b11] == pind) {
    INCREASE_COUNTER
    x4_phase_index = (x4_phase_index + 1) & 0b11;
  } else if (x4_phase_array[(x4_phase_index - 1) & 0b11] == pind) {
    DECREASE_COUNTER
    x4_phase_index = (x4_phase_index - 1) & 0b11;
  } else {
#if ENABLE_PRINTS>0
   // Serial.print("Lost phase synchronization!!! ");
#endif
  }
#endif
}
