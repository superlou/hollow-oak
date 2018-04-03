#include "Arduino.h"
#include "game_state.hpp"
#include "led.hpp"

typedef enum {
  LED_MODE_POWER,
  LED_MODE_COUNT
} LEDMode;

LEDMode mode;

int count;
unsigned long count_time;
int count_duration = 150;
unsigned char count_pattern[] = { B00001010, B10101010, B10101010 };
int count_pos;
int count_end;

void led_init() {
  led_do_power();
  pinMode(LED_BUILTIN, OUTPUT);
}

void led_on() {
  digitalWrite(LED_BUILTIN, LOW);
}

void led_off() {
  digitalWrite(LED_BUILTIN, HIGH);
}

void process_count() {
  unsigned long now = millis();
  if((now - count_time) > count_duration) {
    count_pos++;
    count_time = now;
    if (count_pos >= count_end) {
      count_pos = 0;
    }
  }

  int state = count_pattern[count_pos / 8] & (0x80 >> count_pos % 8);

  if (state == 0) {
    led_off();
  } else {
    led_on();
  }
}

void led_process(void) {
  switch(mode) {
  case LED_MODE_COUNT:
    process_count();
    break;
  case LED_MODE_POWER:
    Serial.println("here!");
  default:
    if (token_is_set(&power_on)) {
      led_on();
    } else {
      led_off();
    }
  }
}

void led_do_power(void) {
  mode = LED_MODE_POWER;
}

void led_do_count(int _count) {
  mode = LED_MODE_COUNT;
  count = _count;
  count_pos = 0;
  count_end = 4 + 2 * count;
  count_time = millis();
}
