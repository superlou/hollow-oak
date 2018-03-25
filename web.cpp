#include <string.h>
#include "memory.hpp"
#include "template.hpp"
#include "eula.hpp"
#include "page_templates.hpp"
#include "token.hpp"
#include "web.hpp"

#define PAGE_TEMPLATE(x) ((char *)page_templates_ ## x ## _html)

ESP8266WebServer server(80);
char tmp[128];

Token eula_accepted;

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

bool get_form_arg(ESP8266WebServer *server, char *arg_name, char *out, int max_length) {
  for ( uint8_t i = 0; i < server->args(); i++ ) {
    if (strcmp(server->argName(i).c_str(), arg_name) == 0) {
      strncpy(out, server->arg(i).c_str(), max_length);
      return true;
    }
  }

  return false;
}

char send_buffer[4096];

char html_template[] = "<!html>"
"<head>"
"</head>"
"<body>"
"%s"
"</body>";

void web_send_html(ESP8266WebServer *server, char *body) {
  snprintf(send_buffer, 4096, html_template, body);
  server->send(200, "text/html", send_buffer);
}

void web_redirect(ESP8266WebServer *server, char *path) {
  server->sendHeader("Location", String(path), true);
  server->send(302, "text/plain", "");
}

char template_buffer[4096];

void index_route(void) {
  if (token_is_set(&eula_accepted)) {
    web_send_html(&server, PAGE_TEMPLATE(index));
  } else {
    web_redirect(&server, "eula");
  }
}

void eula_route(void) {
  web_send_html(&server, PAGE_TEMPLATE(eula_normal));
}

void eula_form(void) {
  char serial[32];
  char accept[8];

  if (get_form_arg(&server, "serial", serial, 32) &&
      get_form_arg(&server, "accept", accept, 8))
  {
    if (strcmp(serial, "1450-3") == 0 &&
        strcmp(accept, "on") == 0)
    {
      token_set(&eula_accepted);
      web_redirect(&server, "/");
      return;
    }
  }

  web_redirect(&server, "eula");
}

void web_setup(void) {
  token_new(&eula_accepted, 0);

  server.on("/", HTTP_GET, index_route);
  server.on("/eula", HTTP_GET, eula_route);
  server.on("/eula", HTTP_POST, eula_form);
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
