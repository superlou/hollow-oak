#ifndef WEB_H_
#define WEB_H_

#include <ESP8266WebServer.h>

extern ESP8266WebServer server;
void handleNotFound(void);
void web_setup(void);
void web_handle_client(void);

#endif
