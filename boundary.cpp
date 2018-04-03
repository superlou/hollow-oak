#include "game_state.hpp"
#include "token.hpp";
#include "web.hpp"
#include "led.hpp"
#include "boundary.hpp"

char emergency[] = "emergency modulation only";
char adjust[] = "ready [<a href=\"/boundary\">adjust</a>]";
char disabled[] = "disabled";

bool adjusting = false;
int boundary = 8;
int boundary_max = 8;

bool order[] = {false, true, true, false, true, false, true, true};

void boundary_get_status(char **out) {
  if (token_is_set(&boundary_disabled)) {
    *out = disabled;
  } else if (token_is_set(&peak_calibration)) {
    *out = adjust;
  } else {
    *out = emergency;
  }
}

void boundary_route(void) {
  if (token_is_set(&boundary_disabled)) {
    web_redirect("/");
  }

  web_render(PAGE_TEMPLATE(boundary));
}

char button_true[] = "quadrature";
char button_false[] = "in-phase";

void boundary_form(void) {
  adjusting = true;
  int pos = boundary_max - boundary;

  if (web_form_arg_present(button_true)) {
    if (order[pos]) {
      boundary--;
    } else {
      boundary++;
    }
  } else if (web_form_arg_present(button_false)) {
    if (!order[pos]) {
      boundary--;
    } else {
      boundary++;
    }
  }

  if (boundary > boundary_max) {
    boundary = boundary_max;
  }

  if (boundary == 0) {
    adjusting = false;
    led_do_power();
    token_set(&boundary_disabled);
    web_redirect("/eula");
    return;
  }

  led_do_count(boundary);
  web_redirect("/boundary");
}
