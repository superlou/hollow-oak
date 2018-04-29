#include "game_state.hpp"

Token eula_accepted;
Token power_on;
Token peak_calibration;
Token boundary_disabled;
Token eula2_passed;
Token cryptogram_solved;
Token quote_solved;
Token morse_passed;
Token music_passed;
Token seen_it;
Token resonance_found;

Token *token_list[TOKEN_COUNT] = {
  &eula_accepted,
  &power_on,
  &peak_calibration,
  &boundary_disabled,
  &eula2_passed,
  &cryptogram_solved,
  &quote_solved,
  &morse_passed,
  &music_passed,
  &seen_it,
  &resonance_found
};

void game_state_init(void) {
  token_new(&eula_accepted, 0, "eula_accepted");
  token_new(&power_on, 1, "power_on");
  token_new(&peak_calibration, 2, "peak_calibration");
  token_new(&boundary_disabled, 3, "boundary_disabled");
  token_new(&eula2_passed, 4, "eula2_passed");
  token_new(&cryptogram_solved, 5, "cryptogram_solved");
  token_new(&quote_solved, 6, "quote_solved");
  token_new(&morse_passed, 7, "morse_passed");
  token_new(&music_passed, 8, "music_passed");
  token_new(&seen_it, 9, "seen_it");
  token_new(&resonance_found, 10, "resonance_found");
}
