#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include "DNSServer.h"
#include "web.hpp"
#include "music.hpp"
#include "led.hpp"
#include "morse.hpp"
#include "game_state.hpp"

#define LOCAL_TEST_MODE

char ssid[] = "PLS-32 MX PORT";

const byte DNS_PORT = 53;
IPAddress ap_ip(10, 10, 10, 1);
DNSServer dns_server;

Token eula_accepted;
Token power_on;
Token peak_calibration;
Token boundary_disabled;
Token eula2_passed;
Token cryptogram_solved;
Token quote_solved;
Token morse_passed;

void game_state_init(void) {
  token_new(&eula_accepted, 0, "eula_accepted");
  token_new(&power_on, 1, "power_on");
  token_new(&peak_calibration, 2, "peak_calibration");
  token_new(&boundary_disabled, 3, "boundary_disabled");
  token_new(&eula2_passed, 4, "eula2_passed");
  token_new(&cryptogram_solved, 5, "cryptogram_solved");
  token_new(&quote_solved, 6, "quote_solved");
  token_new(&morse_passed, 7, "morse_passed");
}

void setup() {
  game_state_init();
  led_init();
  morse_input_init(0);
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
  if (token_is_between(&quote_solved, &morse_passed)) {
    morse_input_process();
    if (morse_is_in_msg()) {
      led_do_off();
    } else {
      led_do_morse();
    }
  }

  if (token_is_set(&morse_passed)) {
    led_do_off();
  }

  music_process();
  led_process();
  dns_server.processNextRequest();
  web_handle_client();
}
