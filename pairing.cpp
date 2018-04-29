#include "web.hpp"
#include "game_state.hpp"
#include "token.hpp"
#include "cstr_util.hpp"
#include "pairing.hpp"

char status_lost[] = "lost";
char status_pairing_1[] = "link in progress, stalled - 7% [<a href='/pairing'>remodulate</a>]";
char status_pairing_2[] = "link in progress, stalled - 63% [<a href='/pairing'>remodulate</a>]";
char status_pairing_3[] = "link in progress, stalled - 98% [a#21re0f='/.ai+iFng'&lt;rDmo-3la";

void pairing_get_status(char **out) {
  if (token_is_set(&quote_solved)) {
    *out = status_pairing_3;
  } else if (token_is_set(&cryptogram_solved)) {
    *out = status_pairing_2;
  } else if (token_is_set(&eula2_passed)) {
    *out = status_pairing_1;
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
    cstr_to_lowercase(token, 32);

    if (strstr(token, "thoreau")) {
      token_set(&cryptogram_solved);
    }

    if (token_is_set(&cryptogram_solved) && strstr(token, "unbearable")) {
      token_set(&quote_solved);
    }
  }

  web_redirect("/");
}
