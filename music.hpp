#ifndef MUSIC_HPP_
#define MUSIC_HPP_

typedef struct {
  int *freqs;
  int *starts;
  int *stops;
  int length;
} Song;

extern Song spider1;
extern Song spider2;
extern Song spider3;
extern Song spider4;
extern Song spider5;

void music_init(int pin);
void music_process(void);
void song_start(Song *song);
void music_note(int freq, int duration_ms);

#endif
