#define SET_LED_HIGH PORTB|=0b00100000
#define SET_LED_LOW PORTB&=0b11011111


void runLed() {
  static Millis t;
  static int state = 0;

  if (motorJamming) {
    SET_LED_HIGH;
    return;
  }

  if (!errorHappened) {
    return;
  }

  switch (state) {
    case 0:
      SET_LED_HIGH;
      t.setTime(100);
      state = 1;
      break;
    case 1:
      if (t.check()) {
        SET_LED_LOW;
        t.setTime(500);
        state = 2;
      }
      break;
    case 2:
      if (t.check()) {
        state = 0;
      }
      break;
  }
}
