#include "Arduino.h"
#include "game_state.hpp"
#include "debug.hpp"

void list_tokens(void) {
  Serial.println("== Tokens ==");
  for (int i = 0; i < TOKEN_COUNT; i++) {
    Serial.print(token_list[i]->addr);
    Serial.print(" ");
    Serial.print(token_list[i]->name);
    Serial.print(" ");
    if (token_is_set(token_list[i])) {
      Serial.println("set");
    } else {
      Serial.println("clear");
    }
  }
}

typedef enum {
  TOKEN_OP_CLEAR,
  TOKEN_OP_SET
} TokenOp;

void change_tokens(char *buffer, TokenOp op) {
  char *token;
  token = strtok(buffer, " ");
  token = strtok(NULL, " ");
  while (token != NULL) {
    int id = atoi(token);
    if ((id >= 0) && (id < TOKEN_COUNT)) {
      if (op == TOKEN_OP_CLEAR) {
        Serial.print("Clearing ");
        Serial.println(id);
        token_clear(token_list[id]);
      } else {
        Serial.print("Setting ");
        Serial.println(id);
        token_set(token_list[id]);
      }
    }
    token = strtok(NULL, " ");
  }
}

void handle_cmd(char *buffer) {
  if (strcmp(buffer, "tokens") == 0) {
    list_tokens();
  } else if (strstr(buffer, "clear ")) {
    change_tokens(buffer, TOKEN_OP_CLEAR);
  } else if (strstr(buffer, "set ")) {
    change_tokens(buffer, TOKEN_OP_SET);
  }
}

void debug_process(void) {
  static char buffer[128];

  if (Serial.available() > 0) {
    int num_chars = Serial.readBytesUntil('\n', buffer, 127);
    buffer[num_chars] = '\0';
    handle_cmd(buffer);
  }
}
