#include "web.hpp"
#include "game_state.hpp"
#include "token.hpp"
#include "eula.hpp"

void eula_route(void) {
  if (token_is_set(&boundary_disabled) && token_is_clear(&eula2_passed)) {
    web_render(PAGE_TEMPLATE(eula2));
  } else {
    web_render(PAGE_TEMPLATE(eula_normal));
  }
}

void check_normal_eula(void) {
  char serial[32];
  char accept[8];

  if (web_get_form_arg("serial", serial, 32) &&
      web_get_form_arg("accept", accept, 8))
  {
    if (strcmp(serial, "1450-3") == 0 &&
        strcmp(accept, "on") == 0)
    {
      token_set(&eula_accepted);
      web_redirect("/");
      return;
    }
  }

  web_redirect("eula");
}

void check_eula2(void) {
  char serial[32];
  char accept[8];

  if (web_get_form_arg("serial", serial, 32) &&
      web_get_form_arg("accept", accept, 8))
  {
    if (strcmp(serial, "emptiness") == 0 &&
        strcmp(accept, "on") == 0)
    {
      token_set(&eula2_passed);
      web_redirect("/");
      return;
    }
  }

  web_redirect("eula");
}

void eula_form(void) {
  if (token_is_set(&boundary_disabled) && token_is_clear(&eula2_passed)) {
    check_eula2();
  } else {
    check_normal_eula();
  }
}
