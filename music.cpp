#include <Arduino.h>
#include "music.hpp"

static int tone_pin;

int s1_freqs[] = {
  392, 523, 523, 523, 587, 659, 659, 659, 587, 523, 587, 659, 523,
  659, 659, 698, 784, 784, 784, 698, 659, 698, 784, 659, 523, 523,
  587, 659, 659, 659, 587, 523, 587, 659, 523, 392, 392, 523, 523,
  523, 587, 659, 659, 659, 587, 523, 587, 659, 523
};
int s1_starts[] = {
  1250, 1499, 1999, 2250, 2750, 3000, 3750, 4250, 4500, 5000, 5250,
  5750, 6000, 7500, 8250, 8750, 9000, 9750, 10250, 10500, 11000,
  11250, 11750, 12000, 13500, 14250, 14750, 15000, 15750, 16250,
  16500, 17000, 17250, 17750, 18000, 18750, 19250, 19500, 20000,
  20250, 20750, 21000, 21750, 22250, 22500, 23000, 23250, 23750, 24000
};
int s1_stops[] = {
  1486, 1973, 2236, 2723, 2986, 3711, 4223, 4486, 4973, 5236, 5723,
  5986, 6711, 8211, 8723, 8986, 9711, 10223, 10486, 10973, 11236,
  11723, 11986, 12711, 14211, 14723, 14986, 15711, 16223, 16486,
  16973, 17236, 17723, 17986, 18711, 19223, 19486, 19973, 20236,
  20723, 20986, 21711, 22223, 22486, 22973, 23236, 23723, 23986, 25423
};

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
}
