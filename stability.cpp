#include "web.hpp"
#include "game_state.hpp"
#include "music.hpp"
#include "stability.hpp"

char no_power[] = "safety minimums";
char borderline[] = "acceptable [<a href=\"stability\">calibrate</a>]";
char peak[] = "peak [<a href=\"stability\">calibrate</a>]";
char corrupted[] = "p#ak3!.\\n";

int magnitude = 0;
int flux = 0;

void stability_get_status(char **out) {
  if (token_is_set(&power_on)) {
    if (token_is_between(&quote_solved, &morse_passed)) {
      *out = corrupted;
    } else if (token_is_set(&peak_calibration)) {
      *out = peak;
    } else {
      *out = borderline;
    }
  } else {
    *out = no_power;
  }
}

void stability_route(void) {
  char magnitude_str[8];
  char flux_str[8];

  if (token_is_set(&seen_it)) {

  } else if (token_is_set(&peak_calibration)) {
    magnitude = 4;
    flux = -18;
  }

  itoa(magnitude, magnitude_str, 10);
  itoa(flux, flux_str, 10);

  if (token_is_set(&seen_it)) {
    web_render(PAGE_TEMPLATE(stability_end), magnitude_str, flux_str);
  } else if (token_is_set(&music_passed)) {
    web_render(PAGE_TEMPLATE(stability_late), magnitude_str, flux_str);
  } else {
    web_render(PAGE_TEMPLATE(stability), magnitude_str, flux_str);
  }
}

void early_game_stability_form(void) {
  char magnitude_str[8];
  char flux_str[8];

  if (!web_get_form_arg("magnitude", magnitude_str, 8) ||
      !web_get_form_arg("flux", flux_str, 8))
  {
    web_redirect("/stability");
  }

  magnitude = atoi(magnitude_str);
  flux = atoi(flux_str);

  if (magnitude == 4 && flux == -18) {
    token_set(&peak_calibration);
  } else {
    token_clear(&peak_calibration);
  }

  web_redirect("/");
}

void late_game_stability_form(void) {
  char magnitude_str[8];
  char flux_str[8];

  if (!web_get_form_arg("magnitude", magnitude_str, 8) ||
      !web_get_form_arg("flux", flux_str, 8))
  {
    web_redirect("/stability");
    return;
  }

  magnitude = atoi(magnitude_str);
  flux = atoi(flux_str);

  if (magnitude == 432 && flux == 7121) {
    token_set(&seen_it);
    magnitude = 4;
    flux = -18;
    web_redirect("/");
    return;
  } else {
    web_redirect("/stability");
    return;
  }

  web_redirect("/");
}

#define MAGNITUDE_RESONANCE 2419
#define FLUX_RESONANCE      8377
#define FREQ_TOO_LOW        422
#define FREQ_TOO_HIGH       844
#define FREQ_CORRECT        640
#define NOTE_DURATION       500
#define BREAK_DURATION      100

void end_game_stability_form(void) {
  char magnitude_str[8];
  char flux_str[8];

  if (!web_get_form_arg("magnitude", magnitude_str, 8) ||
      !web_get_form_arg("flux", flux_str, 8))
  {
    web_redirect("/stability");
    return;
  }

  magnitude = atoi(magnitude_str);
  flux = atoi(flux_str);

  if (magnitude > MAGNITUDE_RESONANCE) {
    music_note(FREQ_TOO_HIGH, NOTE_DURATION);
  } else if (magnitude < MAGNITUDE_RESONANCE) {
    music_note(FREQ_TOO_LOW, NOTE_DURATION);
  } else {
    music_note(FREQ_CORRECT, NOTE_DURATION);
  }

  delay(BREAK_DURATION);

  if (flux > FLUX_RESONANCE) {
    music_note(FREQ_TOO_HIGH, NOTE_DURATION);
  } else if (flux < FLUX_RESONANCE) {
    music_note(FREQ_TOO_LOW, NOTE_DURATION);
  } else {
    music_note(FREQ_CORRECT, NOTE_DURATION);
  }

  if ((magnitude == MAGNITUDE_RESONANCE) and (flux == FLUX_RESONANCE)) {
    token_set(&resonance_found);
    web_redirect("/");
    return;
  }

  web_redirect("/stability");
}

void stability_form(void) {
  if (token_is_set(&seen_it)) {
    end_game_stability_form();
  } else if (token_is_set(&music_passed)) {
    late_game_stability_form();
  } else {
    early_game_stability_form();
  }
}
