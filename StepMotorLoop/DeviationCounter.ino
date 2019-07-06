const float conversion_value = ENCODER_STATE_CHANGE_PER_REV / NEMA_MOTOR_PPR;

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

inline short calculateErrorLoadPositionMode() {
  volatile long  a;
  volatile long  i;

  cli();
  a = internal_encoder_position;
  i = internal_in_stepCounter;
  sei();
  
#if ENABLE_PRINTS>50 and ENABLE_PRINTS<99
  Serial.print(internal_encoder_position); Serial.print(" ");
  Serial.print(converted_motor_position); Serial.println();
#endif

  return a-i;
}


inline u8 checkErrorDirection() {
  bool errorFound = false;
  
#ifdef LOAD_POSITION_CONTROL_MODE
  const short m = calculateErrorLoadPositionMode();
#else
  const float m = calculateErrorStepLossCompensationMode();
#endif

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
