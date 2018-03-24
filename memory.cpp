#include <EEPROM.h>
#include <string.h>
#include "memory.hpp"

#define MATCH 0

#define ID_MAX_LEN  63
char id[ID_MAX_LEN + 1] = "";

void fetch_string(uint32_t addr, uint32_t mem_size, char *str) {
  uint8_t c;
  uint8_t i = 0;

  while((i < (mem_size - 1)) && (c = EEPROM.read(i))) {
    str[i++] = c;
  }

  str[i] = '\0';

  if (strcmp(str, "") == MATCH) {
    strcpy(str, "?");
  }
}

void store_string(const char *str, uint32_t addr, uint32_t mem_size) {
  uint8_t length = strlen(str);
  uint32_t i;

  // Reserve a byte for the null terminator
  if ((length + 1) > mem_size) {
    length = mem_size - 1;
  }

  for (i = 0; i < length; i++) {
    EEPROM.write(addr + i, str[i]);
  }

  EEPROM.write(addr + i, '\0');
  EEPROM.commit();
}

void fetch_id(char *id) {
  fetch_string(0, ID_MAX_LEN + 1, id);
}

void store_id(const char *new_id) {
  store_string(new_id, 0, ID_MAX_LEN + 1);
}

void memory_clear() {
  for (int i = 0 ; i < EEPROM.length(); i++) {
    EEPROM.write(i, 0);
  }
  EEPROM.commit();
}

int memory_length() {
  return EEPROM.length();
}
