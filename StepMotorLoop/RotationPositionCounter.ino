volatile const u8 phase_array[] = {
  0b00110000,
  0b00010000,
  0b00000000,
  0b00100000,
};

volatile char previous_phase_index = 0xff;

//Handle quadrature signals
ISR (PCINT2_vect)
{
  volatile u8 pind = PIND & 0b00110000;
  volatile char current_phase_index;

  //Search phase index
  for (current_phase_index = 0; pind != phase_array[current_phase_index]; current_phase_index++);

  if (((previous_phase_index + 1) & 0b11) == current_phase_index) {
#ifdef FLIP_ENCODER
    internal_encoder_position--;
#else
    internal_encoder_position++;
#endif
  } else if (((previous_phase_index - 1) & 0b11) == current_phase_index) {
#ifdef FLIP_ENCODER
    internal_encoder_position++;
#else
    internal_encoder_position--;
#endif
  } else {
    if (previous_phase_index != 0xff) {
#if ENABLE_PRINTS>0
      Serial.print("Lost phase synchronization!!! ");
      Serial.print((int)current_phase_index);
      Serial.print(",");
      Serial.println((int)previous_phase_index);
#endif
    }
  }

  previous_phase_index = current_phase_index;
}
