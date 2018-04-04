#include "web.hpp"
#include "game_state.hpp"
#include "token.hpp"
#include "pairing.hpp"

char status_lost[] = "lost";
char status_pairing_7[] = "link in progress, stalled - 7% [<a href='/pairing'>remodulate</a>]";
char status_pairing_63[] = "link in progress, stalled - 63% [<a href='/pairing'>remodulate</a>]";

void pairing_get_status(char **out) {
  if (token_is_set(&cryptogram_solved)) {
    *out = status_pairing_63;
  } else if (token_is_set(&eula2_passed)) {
    *out = status_pairing_7;
  } else {
    *out = status_lost;
  }
}

void pairing_route(void) {
  web_render(PAGE_TEMPLATE(pairing));
}

void pairing_form(void) {
  char token[32];

  if (web_get_form_arg("token", token, 32))
  {
    if (strstr(token, "thoreau") || strstr(token, "Thoreau")) {
      token_set(&cryptogram_solved);
    }
  }

  web_redirect("/");
}
