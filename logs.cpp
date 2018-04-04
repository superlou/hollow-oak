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

char list_with_cryptogram_solved[] = "<ol start=\"0\">"
"<li><a href='logs?id=0'>Inspector: Q32. ATP result: pass.</a></li>"
"<li><a href='logs?id=1'>Assigned field agent: FA184. Pairing result: pass.</a></li>"
"<li><a href='logs?id=2'>TRANSCRIPTION [conf 97%]</a></li>"
"<li><a href='logs?id=3'>TRANSCRIPTION [conf 93%]</a></li>"
"<li><a href='logs?id=4'>TRANSCRIPTION [conf 95%]</a></li>"
"<li><a href='logs?id=5'>TRANSCRIPTION [conf 98%]</a></li>"
"<li><a href='logs?id=6'>TRANSCRIPTION [conf 43%]</a></li>"
"<li><a href='logs?id=7'>TRANSCRIPTION [conf 89%]</a></li>"
"<li><a href='logs?id=8'>Automatic relink performed.</a></li>"
"<li><a href='logs?id=9'>TRANSCRIPTION [conf 86%]</a></li>"
"<li><a href='logs?id=10'>TRANSCRIPTION [conf 93%]</a></li>"
"<li><a href='logs?id=11'>TRANSCRIPTION [conf 85%]</a></li>"
"<li><a href='logs?id=12'>TRANSCRIPTION [conf 93%]</a></li>"
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
char log_8a_subject[] = "Log corrupted.";
char log_8a_text[] = "PS C XCD BMLT DMO ULLF FCAL VPOJ JPT AMXFCDPMDT, FLHJCFT PO PT GLACKTL JL JLCHT C BPSSLHLDO BHKXXLH. ZLO JPX TOLF OM OJL XKTPA VJPAJ JL JLCHT, JMVLRLH XLCTKHLB MH SCH CVCI.";
char log_8b_subject[] = "Automatic relink performed.";
char log_8b_text[] = "An automatic re-link has been performed. No user action is necessary.";
char log_9_subject[] = "TRANSCRIPTION [conf 86%]";
char log_9_text[] = "DESK PATROL SUCKS - SOMETHING FUNNY IN THE OFFICE - ALWAYS WHISPERING - I SWEAR I RECOGNIZE THOSE SUITS IN THE CONFERENCE ROOM";
char log_10_subject[] = "TRANSCRIPTION [conf 93%]";
char log_10_text[] = "DAMN RIGHT - THOSE TWO GUYS ARE THE PALETTI BOYS - I PICKED THEM UP 6 MONTHS AGO - WHAT THE HELL ARE THEY DOING HERE";
char log_11_subject[] = "TRANSCRIPTION [conf 85%]";
char log_11_text[] = "JENNY - JENNY. HAVE YOU SEEN MY LINK - THE LITTLE GRAY BOX - I NEED THAT BOX - I NEED IT - TELL ME YOU DIDN'T LOSE IT - I TOLD YOU HOW IMPORTANT IT WAS - WHY WOULD YOU MOVE IT - DAMN IT JENNY - WHY DID YOU MOVE IT - WHERE ARE YOU HIDING IT";
char log_12_subject[] = "TRANSCRIPTION [conf 93%]";
char log_12_text[] = "SHE CAN'T STOP ME. NONE OF THEM CAN STOP ME. I FOUND THE PROOF. CHECK THIS OUT<br><img src='/cat.jpg'>";

LogBody logs[] = {
  { "0", log_0_subject, log_0_text },
  { "1", log_1_subject, log_1_text },
  { "2", log_2_subject, log_2_text },
  { "3", log_3_subject, log_3_text },
  { "4", log_4_subject, log_4_text },
  { "5", log_5_subject, log_5_text },
  { "6", log_6_subject, log_6_text },
  { "7", log_7_subject, log_7_text },
  { "8", log_8a_subject, log_8a_text },
  { "9", log_9_subject, log_9_text },
  { "10", log_10_subject, log_10_text },
  { "11", log_11_subject, log_11_text },
  { "12", log_12_subject, log_12_text },
};

void logs_get_list(char **str) {
  if (token_is_set(&cryptogram_solved)) {
    *str = list_with_cryptogram_solved;
  } else if (token_is_set(&eula2_passed)) {
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

  // This is hacky
  if (token_is_set(&cryptogram_solved)) {
    logs[8].subject = log_8b_subject;
    logs[8].text = log_8b_text;
  }

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
