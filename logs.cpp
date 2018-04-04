#include "game_state.hpp"
#include "web.hpp"
#include "logs.hpp"

char list_no_power[] = "<i>(not accessible - insufficient power)</i>";
char list_with_power[] = "<ol start=\"0\">"
"<li><a href='logs?id=0'>Inspector: Q32. ATP result: pass.</a></li>"
"<li><a href='logs?id=1'>Assigned field agent: FA184. Pairing result: pass.</a></li>"
"<li><a href='logs?id=2'>TRANSCRIPTION [conf 97%]</a></li>"
"<li><i>(remaining logs not accessible)</i></li>"
"</ol>";
char list_with_eula2_passed[] = "<ol start=\"0\">"
"<li><a href='logs?id=0'>Inspector: Q32. ATP result: pass.</a></li>"
"<li><a href='logs?id=1'>Assigned field agent: FA184. Pairing result: pass.</a></li>"
"<li><a href='logs?id=2'>TRANSCRIPTION [conf 97%]</a></li>"
"<li><a href='logs?id=3'>TRANSCRIPTION [conf 93%]</a></li>"
"<li><a href='logs?id=4'>TRANSCRIPTION [conf 95%]</a></li>"
"<li><a href='logs?id=5'>TRANSCRIPTION [conf 98%]</a></li>"
"<li><a href='logs?id=6'>TRANSCRIPTION [conf 43%]</a></li>"
"<li><a href='logs?id=7'>TRANSCRIPTION [conf 89%]</a></li>"
"<li><a href='logs?id=8'>Log corrupted.</a></li>"
"<li><i>(remaining memory corrupted)</i></li>"
"</ol>";

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
char log_2_text[] = "HELLO - IS THIS THING ON - CHECK 1 - CHECK 2 - HEY - WHERE DO YOU THINK YOU'RE PUTTING -";
char log_3_subject[] = "TRANSCRIPTION [conf 93%]";
char log_3_text[] = "ANOTHER PIECE OF GEAR I NEED TO CHARGE EVERY NIGHT - GREAT - END RECORDING - CUT RECORDING - DAMN IT - THAT'S RIGHT";
char log_4_subject[] = "TRANSCRIPTION [conf 95%]";
char log_4_text[] = "SO WE CAN'T USE STILLS AS EVIDENCE - WORTHLESS VENDOR CRAP - BUT AT LEAST I CAN KEEP THIS WITH ME<br><img src='/cat.jpg'>";
char log_5_subject[] = "TRANSCRIPTION [conf 98%]";
char log_5_text[] = "NOTE TO SELF - GOT CHEWED OUT FOR LEAVING THIS P-O-S HOME - ALMOST WORTH IT";
char log_6_subject[] = "TRANSCRIPTION [conf 43%]";
char log_6_text[] = "OH MY GOD - IT [UNKNOWN] WORKED - I'M ABOUT TO GO OFF SHIFT AND THE RADIO [UNKNOWN-EXTENDED] ACROSS THE STREET. GET OUT OF MY CAR AND THE DRUNKS HOLDING UP THE CASHIER WITH AN UMBRELLA. I GO IN REAL SLOW, HANDS UP TO REASON WITH THIS [UNKNOWN] HE [UNKNOWN-GUESS DRAWERS] GUN - I'M STANDING THERE WITH MY HANDS [UNKNOWN] - AND THEN I HEAR IT - HEAR HIM - BUT HE'S NOT SPEAKING - I'M [UNKNOWN, EST: BUCKING] IN HIS MIND. HE JUST WANTS TO GO BACK TO HIS ANDREA - AND I REMIND HIM - AND HE JUST BREAKS DOWN SOBBING - SHIT MAN";
char log_7_subject[] = "TRANSCRIPTION [conf 89%]";
char log_7_text[] = "ALMOST FORGET TO BUY JENNY FLOWERS - BUT - SHE THINKS ABOUT OUR ANNIVERSARY - A LOT - I'M THE MAN";
char log_8_subject[] = "Log corrupted.";
char log_8_text[] = "PS C XCD BMLT DMO ULLF FCAL VPOJ JPT AMXFCDPMDT, FLHJCFT PO PT GLACKTL JL JLCHT C BPSSLHLDO BHKXXLH. ZLO JPX TOLF OM OJL XKTPA VJPAJ JL JLCHT, JMVLRLH XLCTKHLB MH SCH CVCI.";

LogBody logs[] = {
  { "0", log_0_subject, log_0_text },
  { "1", log_1_subject, log_1_text },
  { "2", log_2_subject, log_2_text },
  { "3", log_3_subject, log_3_text },
  { "4", log_4_subject, log_4_text },
  { "5", log_5_subject, log_5_text },
  { "6", log_6_subject, log_6_text },
  { "7", log_7_subject, log_7_text },
  { "8", log_8_subject, log_8_text },
};

void logs_get_list(char **str) {
  if (token_is_set(&eula2_passed)) {
    *str = list_with_eula2_passed;
  } else if (token_is_set(&power_on)) {
    *str = list_with_power;
  } else {
    *str = list_no_power;
  }
}

void logs_route(void) {
  char id_str[8];
  int id;

  if (!web_get_form_arg("id", id_str, 8)) {
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
    web_redirect("/");
  } else {
    web_render(PAGE_TEMPLATE(log), id_str, logs[index].subject, logs[index].text);
  }
}
