#include <string.h>
#include "memory.hpp"
#include "web.hpp"
#include "template.hpp"
#include "eula.hpp"
#include "page_templates.hpp"

#define PAGE_TEMPLATE(x) ((char *)page_templates_ ## x ## _html)

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
  web_send_html(&server, PAGE_TEMPLATE(index));

  // web_redirect(&server, "eula");
}

void eula_route(void) {
  web_send_html(&server, PAGE_TEMPLATE(eula_normal));
}

void web_setup(void) {
  server.on("/", HTTP_GET, index_route);
  server.on("/eula", HTTP_GET, eula_route);
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
