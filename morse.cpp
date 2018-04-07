#include "Arduino.h"
#include "morse.hpp"
#include "game_state.hpp"

typedef enum {
  DOWN, UP
} KeyState;

int pin;
KeyState last_key_state;

void morse_input_init(int _pin) {
  pin = _pin;
  pinMode(pin, INPUT);
  last_key_state = UP;
}

bool key_is_down() {
  return (digitalRead(0) == 0);
}

bool key_is_up() {
  return !key_is_down();
}

bool in_msg = false;
unsigned long time_last_change;
unsigned long msg_break_gap = 2000;
unsigned int spans[256];  // todo This should be protected from overflow
int span_index = 0;

bool morse_is_in_msg() {
  return in_msg;
}

/**
 * Note that this cannot determine word breaks
 */
void decode_morse_spans(unsigned int *spans, int len, char *msg) {
  // Serial.println("message stopped");
  // for (int i = 0; i < len; i++) {
  //   Serial.println(spans[i]);
  // }

  // Make a dit-dah threshold from the minimum gap
  int min = 2000;
  for (int i = 1; i < len; i += 2) {
    if (spans[i] < min) {
      min = spans[i];
    }
  }

  int threshold = min * 2.5;

  // Determine dits and dahs
  int msg_index = 0;

  for (int i = 0; i < len; i += 2) {
    if (spans[i] > threshold) {
      msg[msg_index++] = '-';
    } else {
      msg[msg_index++] = '.';
    }
  }

  msg[msg_index] = '\0';
}

bool morse_input_process() {
  int now = millis();

  if (key_is_down()) {
    if (last_key_state == UP && (now - time_last_change > 50)) {
      if (in_msg) {
        spans[span_index++] = now - time_last_change;
      }
      time_last_change = now;
    }

    in_msg = true;
    last_key_state = DOWN;
  } else {
    if (last_key_state == DOWN && (now - time_last_change > 50)) {
      spans[span_index++] = now - time_last_change;
      time_last_change = now;
    }

    last_key_state = UP;
  }

  // Check for msg break
  char msg[128];

  if (((now - time_last_change) > msg_break_gap) && in_msg) {
    decode_morse_spans(spans, span_index, msg);
    Serial.println(msg);
    in_msg = false;
    span_index = 0;
    if (strcmp(msg, "-.--....") == 0) {
      token_set(&morse_passed);
      return true;
    }
  }

  return false;
}
