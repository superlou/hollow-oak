#include <Arduino.h>
#include "music.hpp"

static int tone_pin;

int spider1_freqs[] = {
  392, 523, 523, 523, 587, 659, 659
};
int spider1_starts[] = {
  0, 249, 749, 1000, 1500, 1750, 2500
};
int spider1_stops[] = {
  236, 723, 986, 1473, 1736, 2461, 2973
};
int spider2_freqs[] = {
  659, 587, 523, 587, 659, 523
};
int spider2_starts[] = {
  0, 249, 749, 1000, 1500, 1750
};
int spider2_stops[] = {
  236, 723, 986, 1473, 1736, 2461
};
int spider3_freqs[] = {
  659, 659, 698, 784
};
int spider3_starts[] = {
  0, 750, 1250, 1499
};
int spider3_stops[] = {
  711, 1223, 1486, 2686
};
int spider4_freqs[] = {
  784, 698, 659, 698, 784, 659
};
int spider4_starts[] = {
  0, 249, 749, 1000, 1500, 1750
};
int spider4_stops[] = {
  236, 723, 986, 1473, 1736, 2461
};
int spider5_freqs[] = {
  523, 523, 587, 659
};
int spider5_starts[] = {
  0, 750, 1250, 1499
};
int spider5_stops[] = {
  711, 1223, 1486, 2686
};

Song spider1 = {
  spider1_freqs, spider1_starts, spider1_stops, sizeof(spider1_starts)/sizeof(spider1_starts[0])
};

Song spider2 = {
  spider2_freqs, spider2_starts, spider2_stops, sizeof(spider2_starts)/sizeof(spider2_starts[0])
};

Song spider3 = {
  spider3_freqs, spider3_starts, spider3_stops, sizeof(spider3_starts)/sizeof(spider3_starts[0])
};

Song spider4 = {
  spider4_freqs, spider4_starts, spider4_stops, sizeof(spider4_starts)/sizeof(spider4_starts[0])
};

Song spider5 = {
  spider5_freqs, spider5_starts, spider5_stops, sizeof(spider5_starts)/sizeof(spider5_starts[0])
};

Song *now_playing = NULL;
int now_playing_start;
int now_playing_index;

void music_init(int pin) {
  tone_pin = pin;
}

void start_tone(int freq) {
  analogWriteFreq(freq);
  analogWrite(tone_pin, 512);
}

void stop_tone() {
  analogWrite(tone_pin, 0);
}

void music_note(int freq, int duration_ms) {
  start_tone(freq);
  delay(duration_ms);
  stop_tone();
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
