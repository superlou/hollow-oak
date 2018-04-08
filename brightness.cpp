#include "Arduino.h"
#include "brightness.hpp"

static int brightness_pin;

void brightness_init(int pin) {
  brightness_pin = pin;
  pinMode(brightness_pin, INPUT_PULLUP);
}

// GL5516 has a good threshold
bool brightness_is_dark(void) {
  int result = digitalRead(brightness_pin);
  return result ? true : false;
}
