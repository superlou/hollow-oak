#ifndef MUSIC_HPP_
#define MUSIC_HPP_

typedef struct {
  int *freqs;
  int *starts;
  int *stops;
  int length;
} Song;

void music_init(int pin);
void music_process(void);
void song_start(Song *song);

#endif
