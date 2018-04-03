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
#include "web.hpp"

ESP8266WebServer server(80);
char tmp[128];

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

char send_buffer[4096];

void web_render(char *body, ...) {
  va_list argptr;
  va_start(argptr, body);
  vsnprintf(send_buffer, 4096, body, argptr);
  va_end(argptr);
  server.send(200, "text/html", send_buffer);
}

void web_redirect(char *path) {
  server.sendHeader("Location", String(path), true);
  server.send(302, "text/plain", "");
}

char template_buffer[4096];

void index_route(void) {
  if (token_is_clear(&eula_accepted)) {
    web_redirect("eula");
    return;
  }

  char power_status[64];
  power_status_msg(power_status);

  char *log_list;
  logs_get_list(&log_list);

  char *stability_status;
  stability_get_status(&stability_status);

  char *boundary_status;
  boundary_get_status(&boundary_status);

  web_render(PAGE_TEMPLATE(index), power_status, stability_status,
                boundary_status, log_list);
}

void eula_route(void) {
  web_render(PAGE_TEMPLATE(eula_normal));
}

void eula_form(void) {
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

void cat_route(void) {
  server.sendContent_P((char *)page_templates_cat_jpg, page_templates_cat_jpg_len - 1);
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
  server.on("/cat.jpg", HTTP_GET, cat_route);
  server.on("/reset", HTTP_GET, [](){
    memory_clear();
    server.send(200, "text/plain", "Memory cleared.");
  });
  server.onNotFound(handleNotFound);

  server.begin();
}

void web_handle_client(void) {
  server.handleClient();
}
