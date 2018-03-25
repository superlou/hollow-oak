#ifndef WEB_H_
#define WEB_H_

#include <ESP8266WebServer.h>
#include "page_templates.hpp"

#define PAGE_TEMPLATE(x) ((char *)page_templates_ ## x ## _html)

extern ESP8266WebServer server;
void handleNotFound(void);
void web_setup(void);
void web_handle_client(void);
void web_send_html(ESP8266WebServer *server, char *body, ...);
void web_redirect(ESP8266WebServer *server, char *path);
bool web_get_form_arg(ESP8266WebServer *server, char *arg_name, char *out, int max_length);
bool web_form_arg_present(ESP8266WebServer *server, char *arg_name);

#endif
