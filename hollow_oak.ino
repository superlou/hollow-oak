#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include "DNSServer.h"
#include "web.hpp"
#include "music.hpp"
#include "game_state.hpp"

#define LOCAL_TEST_MODE

char ssid[] = "PLS-32 MX PORT";

const byte DNS_PORT = 53;
IPAddress ap_ip(10, 10, 10, 1);
DNSServer dns_server;

Token eula_accepted;
Token power_on;

void game_state_init(void) {
  token_new(&eula_accepted, 0, "eula_accepted");
  token_new(&power_on, 1, "power_on");
}

void setup() {
  game_state_init();
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  EEPROM.begin(256);

  #ifndef LOCAL_TEST_MODE
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(ap_ip, ap_ip, IPAddress(255, 255, 255, 0));
    WiFi.softAP(ssid);
    dns_server.start(DNS_PORT, "*", ap_ip);
  #else
    WiFi.begin("meta_2g4", "galactica");
    Serial.print("Connecting to network");

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }

    Serial.println();

    Serial.print("Connected, IP address: ");
    Serial.println(WiFi.localIP());
  #endif

  web_setup();
  music_init(14);
}

void loop() {
  music_process();
  dns_server.processNextRequest();
  web_handle_client();
}
