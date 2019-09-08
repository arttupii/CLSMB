#ifdef X4_ENCODING
volatile const u8 x4_phase_array[] = {
  0b00001100,
  0b00000100,
  0b00000000,
  0b00001000,
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


volatile u8 prevValue;
//Handle quadrature signals
ISR (INT0_vect) //A-Signal
{ //76543210
  volatile u8 pind = PIND & 0b00001100;

#ifdef X1_ENCODING
  if (pind & 0b00001000) {
    INCREASE_COUNTER;
  } else {
    DECREASE_COUNTER
  }
#endif
#ifdef X2_ENCODING
  if (pind == 0b1000 || pind == 0b0100) {
    DECREASE_COUNTER
  }
  if (pind == 0b1100 || pind == 0b0000) {
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


#ifdef X4_ENCODING
ISR (INT1_vect)
{
  volatile u8 pind = PIND & 0b00001100;

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
}
#endif
