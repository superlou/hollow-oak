#include <Arduino.h>
#include "music.hpp"

static int tone_pin;

int s1_freqs[] = {523, 659, 698, 784, 880, 1047};
int s1_starts[] = {0, 100, 500, 800, 1500, 1550};
int s1_stops[] = {80, 200, 700, 1200, 1550, 2000};
int s1_length = sizeof(s1_starts)/sizeof(s1_starts[0]);
Song s1;

Song *now_playing = NULL;
int now_playing_start;
int now_playing_index;

void music_init(int pin) {
  tone_pin = pin;

  s1.freqs = s1_freqs;
  s1.starts = s1_starts;
  s1.stops = s1_stops;
  s1.length = s1_length;
  song_start(&s1);
}

void start_tone(int freq) {
  analogWriteFreq(freq);
  analogWrite(tone_pin, 512);
}

void stop_tone() {
  analogWrite(tone_pin, 0);
}

int freqs[] = {523, 587, 659, 698, 784, 880, 988, 1047};
char num_freqs = sizeof(freqs) / sizeof(freqs[0]);
int note = 0;

void song_start(Song *song) {
  now_playing = song;
  now_playing_index = 0;
  now_playing_start = millis();
}

void song_stop() {
  stop_tone();
  now_playing = NULL;
}

void music_process(void) {
  if (now_playing) {
    int song_time = millis() - now_playing_start;

    if (song_time > now_playing->stops[now_playing_index]) {
      stop_tone();
      now_playing_index++;

      if (now_playing_index > now_playing->length) {
        song_stop();
        return;
      }
    }

    if (song_time > now_playing->starts[now_playing_index]) {
      start_tone(now_playing->freqs[now_playing_index]);
    }
  }

  // start_tone(freqs[note++]);
  // note %= num_freqs;
  // delay(1000);
  // stop_tone();
  // delay(100);
}
