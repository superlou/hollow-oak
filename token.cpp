#include <EEPROM.h>
#include "memory.hpp"
#include "token.hpp"

void token_new(Token *token, int addr, char* name) {
  token->addr = addr;
  strcpy(token->name, name);
}

bool token_is_set(Token *token) {
  uint8_t i = EEPROM.read(token->addr);
  return i ? true : false;
}

bool token_is_clear(Token *token) {
  return !token_is_set(token);
}

bool token_is_between(Token *complete, Token *incomplete) {
  return (token_is_set(complete) && token_is_clear(incomplete));
}

void token_set(Token *token) {
  EEPROM.write(token->addr, true);
  EEPROM.commit();
}

void token_clear(Token *token) {
  EEPROM.write(token->addr, false);
  EEPROM.commit();
}
