#include "Arduino.h"
#include "game_state.hpp"
#include "led.hpp"

typedef enum {
  LED_MODE_POWER,
  LED_MODE_COUNT,
  LED_MODE_MORSE,
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

unsigned long morse_time;
int morse_dit_duration = 150;
unsigned char morse_pattern[] = {
  //          c             ___a       ___n        ====   ===y
  B00000000, B11101011, B10100010, B11100011, B10100000, B00011101,
  //          ___o             __   _u         =======h         ___
  B01110111, B00011101, B11011100, B01010111, B00000001, B01010100,
  //e___l           __   _p             ====   ===m         ___e
  B01000101, B11010100, B01011101, B11010000, B00011101, B11000100,
  //pause
  B00000000, B00000000, B00000000
};
int morse_pos;
int morse_end = 21 * 8;

void process_morse() {
  unsigned long now = millis();
  if((now - morse_time) > morse_dit_duration) {
    morse_pos++;
    morse_time = now;
    if (morse_pos > morse_end) {
      morse_pos = 0;
    }
  }

  int state = morse_pattern[morse_pos / 8] & (0x80 >> morse_pos % 8);

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
  case LED_MODE_MORSE:
    process_morse();
    break;
  case LED_MODE_POWER:
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

void led_do_morse(void) {
  mode = LED_MODE_MORSE;
  morse_pos = 0;
  morse_time = millis();
}
