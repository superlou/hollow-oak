#include <EEPROM.h>
#include "memory.hpp"
#include "token.hpp"

void token_new(Token *token, int addr) {
  token->addr = addr;
}

bool token_is_set(Token *token) {
  uint8_t i = EEPROM.read(token->addr);
  return i ? true : false;
}

void token_set(Token *token) {
  EEPROM.write(token->addr, true);
  EEPROM.commit();
}

void token_clear(Token *token) {
  EEPROM.write(token->addr, false);
  EEPROM.commit();
}
