const float conversion_value = ENCODER_STATE_CHANGE_PER_REV / NEMA_MOTOR_PPR;

#ifdef STEP_LOSS_COMPENSATION_MODE
inline float calculateErrorStepLossCompensationMode() {
  volatile float  a;
  volatile float  i;

  cli();
  a = internal_encoder_position;
  i = internal_in_stepCounter;
  sei();
  float converted_motor_position = i * conversion_value;
  float ret = a - converted_motor_position;

#if ENABLE_PRINTS>50 and ENABLE_PRINTS<99
  Serial.print(internal_encoder_position); Serial.print(" ");
  Serial.print(converted_motor_position); Serial.println();
#endif

  return ret;
}

inline u8 checkErrorDirection() {
  bool errorFound = false;

  const float m = calculateErrorStepLossCompensationMode();

  if (m < -STEP_ERROR_MAX || m > STEP_ERROR_MAX) {
    errorFound = true;
  } else if (m >= -STEP_ERROR_MIN && m <= STEP_ERROR_MIN) {
    return 0;
  }

  if (errorFound) {
    if (m < 0) {
      return 2;
    } else {
      return 1;
    }
  }

  return 0; //OK
}
#endif
