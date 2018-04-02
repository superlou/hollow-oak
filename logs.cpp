#include "game_state.hpp"
#include "web.hpp"
#include "logs.hpp"

char list_no_power[] = "<i>(inadequate power - not accessible)</i>";
char list_with_power[] = "<ul>"
"<li><a href='logs?id=0'>0 - Inspector: Q32. ATP result: pass.</a></li>"
"<li><a href='logs?id=1'>1 - Assigned field agent: FA184. Pairing result: pass.</a></li>"
"<li><a href='logs?id=2'>2 - TRANSCRIPTION [conf 97%]</a></li>"
"</ul>";

typedef struct {
  char id[3];
  char* subject;
  char* text;
} LogBody;

char log_0_subject[] = "Inspector: Q32. ATP result: pass.";
char log_0_text[] = "Field stabilization 4 dB below nominal. Flux polarity reversion 18 Hz above nominal. Recal not required.";
char log_1_subject[] = "Assigned field agent: FA184. Pairing result: pass.";
char log_1_text[] = "Blood type A-, resting 68 BPM, peak 170 BPM. Initial pairing failed tolerance for beta2, beta3. Two doses 10 mg dextroamphetamine sulfate, 4 hours apart, pairing attempt successful.";
char log_2_subject[] = "TRANSCRIPTION [conf 97%]";
char log_2_text[] = "HELLO - IS THIS THING ON - CHECK 1 - CHECK 2 - HEY - WHERE IS THAT SUPPOSED TO -";

LogBody logs[] = {
  { "0", log_0_subject, log_0_text },
  { "1", log_1_subject, log_1_text },
  { "2", log_2_subject, log_2_text }
};

void logs_get_list(char **str) {
  if (token_is_set(&power_on)) {
    *str = list_with_power;
  } else {
    *str = list_no_power;
  }
}

void logs_route(void) {
  char id_str[8];
  int id;

  if (!web_get_form_arg(&server, "id", id_str, 8)) {
    return;
  }

  int index = -1;

  for (int i = 0; i < sizeof(logs)/sizeof(logs[0]); i++) {
    if (strcmp(id_str, logs[i].id) == 0) {
      index = i;
      break;
    }
  }

  if (index == -1) {
    web_redirect(&server, "/");
  } else {
    web_send_html(&server, PAGE_TEMPLATE(log), id_str, logs[index].subject, logs[index].text);
  }
}
