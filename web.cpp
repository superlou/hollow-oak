#include <string.h>
#include <Arduino.h>
#include "memory.hpp"
#include "template.hpp"
#include "game_state.hpp"
#include "token.hpp"
#include "power.hpp"
#include "logs.hpp"
#include "stability.hpp"
#include "boundary.hpp"
#include "eula.hpp"
#include "pairing.hpp"
#include "led.hpp"
#include "brightness.hpp"
#include "music.hpp"
#include "web.hpp"

ESP8266WebServer server(80);
char tmp[128];

void index_music_form(void);

void handleNotFound(void) {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for ( uint8_t i = 0; i < server.args(); i++ ) {
    message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
  }

  server.send(404, "text/plain", message);
}

bool web_get_form_arg(char *arg_name, char *out, int max_length) {
  for (uint8_t i = 0; i < server.args(); i++) {
    if (strcmp(server.argName(i).c_str(), arg_name) == 0) {
      strncpy(out, server.arg(i).c_str(), max_length);
      return true;
    }
  }

  return false;
}

bool web_form_arg_present(char *arg_name) {
  for (uint8_t i = 0; i < server.args(); i++) {
    if (strcmp(server.argName(i).c_str(), arg_name) == 0) {
      return true;
    }
  }

  return false;
}

#define TEMPLATE_BUFFER_LEN 8192

char send_buffer[TEMPLATE_BUFFER_LEN];

void web_render(char *body, ...) {
  va_list argptr;
  va_start(argptr, body);
  vsnprintf_P(send_buffer, TEMPLATE_BUFFER_LEN, body, argptr);
  va_end(argptr);
  server.send(200, "text/html", send_buffer);
}

void web_redirect(char *path) {
  server.sendHeader("Location", String(path), true);
  server.send(302, "text/plain", "");
}

char template_buffer[TEMPLATE_BUFFER_LEN];

void index_route(void) {
  if (token_is_clear(&eula_accepted)) {
    web_redirect("eula");
    return;
  }

  if (token_is_between(&boundary_disabled, &eula2_passed)) {
    web_redirect("eula");
    return;
  }

  if (token_is_between(&quote_solved, &morse_passed)) {
    led_do_morse();
  }

  if (token_is_between(&morse_passed, &resonance_found)) {
    if (!brightness_is_dark()){
      web_redirect("eula");
      return;
    }

    if (token_is_set(&seen_it)) {
      web_render(PAGE_TEMPLATE(index_seen_it));
      return;
    } else if (token_is_set(&music_passed)) {
      web_render(PAGE_TEMPLATE(index_thanks));
      return;
    } else if (token_is_between(&morse_passed, &music_passed)){
      web_render(PAGE_TEMPLATE(index_music));
      return;
    }
  }

  if (token_is_set(&resonance_found)) {
    web_render(PAGE_TEMPLATE(index_overloading));
    return;
  }

  char power_status[64];
  power_status_msg(power_status);

  char *pairing_status;
  pairing_get_status(&pairing_status);

  char *log_list;
  logs_get_list(&log_list);

  char *stability_status;
  stability_get_status(&stability_status);

  char *boundary_status;
  boundary_get_status(&boundary_status);

  web_render(PAGE_TEMPLATE(index), power_status, pairing_status,
             stability_status, boundary_status, log_list);
}

void cat_route(void) {
  server.sendContent_P((char *)images_cat_jpg, images_cat_jpg_len - 1);
}

void image_quote_route(void) {
  server.sendContent_P((char *)images_quote_jpg, images_quote_jpg_len - 1);
}

void web_setup(void) {
  server.on("/", HTTP_GET, index_route);
  server.on("/eula", HTTP_GET, eula_route);
  server.on("/eula", HTTP_POST, eula_form);
  server.on("/power", HTTP_GET, power_route);
  server.on("/power", HTTP_POST, power_form);
  server.on("/logs", HTTP_GET, logs_route);
  server.on("/stability", HTTP_GET, stability_route);
  server.on("/stability", HTTP_POST, stability_form);
  server.on("/boundary", HTTP_GET, boundary_route);
  server.on("/boundary", HTTP_POST, boundary_form);
  server.on("/pairing", HTTP_GET, pairing_route);
  server.on("/pairing", HTTP_POST, pairing_form);
  server.on("/music", HTTP_POST, index_music_form);
  server.on("/IMG_20180401_063810.jpg", HTTP_GET, cat_route);
  server.on("/IMG_20180404_191543.jpg", HTTP_GET, image_quote_route);
  server.on("/reset", HTTP_GET, [](){
    memory_clear();
    server.send(200, "text/plain", "Memory cleared.");
  });
  server.on("/beep", HTTP_GET, [](){
    music_note(1000, 100);
    server.send(200, "text/plain", "Beeped.");
  });
  server.on("/led/blink/slow", HTTP_GET, [](){
    led_do_blink(500);
    server.send(200, "text/plain", "Blinking.");
  });
  server.on("/led/blink/fast", HTTP_GET, [](){
    led_do_blink(100);
    server.send(200, "text/plain", "Blinking.");
  });
  server.on("/led/on", HTTP_GET, [](){
    led_do_on();
    server.send(200, "text/plain", "On.");
  });
  server.on("/led/off", HTTP_GET, [](){
    led_do_off();
    server.send(200, "text/plain", "Off.");
  });
  server.onNotFound(handleNotFound);

  server.begin();
}

void web_handle_client(void) {
  server.handleClient();
}

char music_pass[] = "12345212";
char music_entry[] = "        ";
unsigned char music_pass_len = sizeof(music_pass);

void index_music_form(void) {
  memmove(music_entry, music_entry + 1, music_pass_len - 2);
  // music_entry[music_pass_len] = '\0';

  if (web_form_arg_present("a")) {
    song_start(&spider3);
    music_entry[music_pass_len - 2] = '3';
  } else if (web_form_arg_present("b")) {
    song_start(&spider4);
    music_entry[music_pass_len - 2] = '4';
  } else if (web_form_arg_present("c")) {
    song_start(&spider2);
    music_entry[music_pass_len - 2] = '2';
  } else if (web_form_arg_present("d")) {
    song_start(&spider1);
    music_entry[music_pass_len - 2] = '1';
  } else if (web_form_arg_present("e")) {
    song_start(&spider5);
    music_entry[music_pass_len - 2] = '5';
  }

  if (strcmp(music_entry, music_pass) == 0) {
    token_set(&music_passed);
  }

  web_redirect("/");
}
