#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include "DNSServer.h"
#include "web.hpp"
#include "music.hpp"
#include "led.hpp"
#include "morse.hpp"
#include "debug.hpp"
#include "brightness.hpp"
#include "game_state.hpp"

// #define LOCAL_TEST_MODE

char ssid1[] = "PLS-32 MX PORT";
char ssid2[] = "hollow.oak";

const byte DNS_PORT = 53;
IPAddress ap_ip(10, 10, 10, 1);
DNSServer dns_server;

void start_ap(void) {
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(ap_ip, ap_ip, IPAddress(255, 255, 255, 0));

  if (token_is_set(&morse_passed)) {
    WiFi.softAP(ssid2);
  } else {
    WiFi.softAP(ssid1);
  }

  dns_server.start(DNS_PORT, "*", ap_ip);
}

void change_ap_to(char *ssid) {
  WiFi.softAP(ssid);
}

void setup() {
  game_state_init();
  led_init();
  morse_input_init(0);
  brightness_init(4);
  Serial.begin(115200);
  EEPROM.begin(256);

  #ifndef LOCAL_TEST_MODE
    start_ap();
  #else
    WiFi.begin("ssid", "password");
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
  if (token_is_between(&quote_solved, &morse_passed)) {
    bool morse_just_passed = morse_input_process();

    if (morse_just_passed) {
      #ifndef LOCAL_TEST_MODE
        change_ap_to(ssid2);
      #endif
    }

    if (morse_is_in_msg()) {
      led_do_off();
    } else {
      led_do_morse();
    }
  }

  if (token_is_between(&morse_passed, &resonance_found)) {
    led_do_off();
  }

  music_process();
  led_process();
  dns_server.processNextRequest();
  web_handle_client();
  debug_process();
  brightness_is_dark();
}
