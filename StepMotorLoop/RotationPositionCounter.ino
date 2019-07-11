volatile const u8 phase_array[] = {
  0b00110000,
  0b00010000,
  0b00000000,
  0b00100000,
};

//Handle quadrature signals
ISR (PCINT2_vect)
{
  volatile u8 pind = PIND & 0b00110000;
  volatile u8 previous_phase_index;

  previous_phase_index = ((u8)internal_encoder_position)&0b11;
  
  if (phase_array[(previous_phase_index+1)&0b11]==pind) {
#ifdef FLIP_ENCODER
    internal_encoder_position--;
#else
    internal_encoder_position++;
#endif
  } else if (phase_array[(previous_phase_index-1)&0b11]==pind) {
#ifdef FLIP_ENCODER
    internal_encoder_position++;
#else
    internal_encoder_position--;
#endif
  } else {
#if ENABLE_PRINTS>0
      Serial.print("Lost phase synchronization!!! ");
#endif
  }
}
