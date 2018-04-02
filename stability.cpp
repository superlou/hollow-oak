#include "web.hpp"
#include "game_state.hpp"
#include "stability.hpp"

char no_power[] = "safety minimums";
char borderline[] = "acceptable [<a href=\"stability\">calibrate</a>]";
char peak[] = "peak [<a href=\"stability\">calibrate</a>]";

int magnitude = 0;
int flux = 0;

void stability_get_status(char **out) {
  if (token_is_set(&power_on)) {
    if (token_is_set(&peak_calibration)) {
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

  if (token_is_set(&peak_calibration)) {
    magnitude = 4;
    flux = -18;
  }

  itoa(magnitude, magnitude_str, 10);
  itoa(flux, flux_str, 10);

  web_send_html(&server, PAGE_TEMPLATE(stability), magnitude_str, flux_str);
}

void stability_form(void) {
  char magnitude_str[8];
  char flux_str[8];

  if (!web_get_form_arg(&server, "magnitude", magnitude_str, 8) ||
      !web_get_form_arg(&server, "flux", flux_str, 8))
  {
    web_redirect(&server, "/stability");
  }

  magnitude = atoi(magnitude_str);
  flux = atoi(flux_str);

  if (magnitude == 4 && flux == -18) {
    token_set(&peak_calibration);
  } else {
    token_clear(&peak_calibration);
  }

  web_redirect(&server, "/");
}
