void runLed() {
  static Millis t;
  static int state = 0;

  if (motorJamming) {
    digitalWrite(LED, HIGH);
    return;
  }

  if (!errorHappened) {
    return;
  }

  switch (state) {
    case 0:
      digitalWrite(LED, HIGH);
      t.setTime(100);
      state = 1;
      break;
    case 1:
      if (t.check()) {
        digitalWrite(LED, LOW);
        t.setTime(500);
        state = 2;
      }
    case 2:
      if (t.check()) {
        state = 0;
      }
      break;
  }
}
