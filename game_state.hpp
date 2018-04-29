#ifndef GAME_STATE_HPP_
#define GAME_STATE_HPP_

#include "token.hpp"

extern Token eula_accepted;
extern Token power_on;
extern Token peak_calibration;
extern Token boundary_disabled;
extern Token eula2_passed;
extern Token cryptogram_solved;
extern Token quote_solved;
extern Token morse_passed;
extern Token music_passed;
extern Token seen_it;
extern Token resonance_found;

#define TOKEN_COUNT 11
extern Token *token_list[TOKEN_COUNT];

void game_state_init(void);

#endif
