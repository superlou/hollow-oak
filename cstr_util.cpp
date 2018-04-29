#include <ctype.h>
#include "cstr_util.hpp"

void cstr_to_lowercase(char *str, int len) {
  for (int i = 0; i < len; i++) {
    str[i] = tolower(str[i]);
  }
}
