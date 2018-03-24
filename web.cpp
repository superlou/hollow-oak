#include <string.h>
#include "memory.hpp"
#include "web.hpp"
#include "template.hpp"

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

char out[4096];

void index_route(void) {
  t_new(out);
  t_header(out);
  t_h1(out, "hollow.oak");
  t_link(out, "/reset", "reset");
  t_footer(out);
  server.send(200, "text/html", out);
}

void web_setup(void) {
  server.on("/", HTTP_GET, index_route);
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
