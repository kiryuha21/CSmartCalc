#include "../s21_smart_calc.h"

void safe_solo_char_replace(char* str, char sym, char replacer) {
  char* search = strchr(str, sym);
  if (search != NULL) {
    size_t pos = search - str;
    str[pos] = replacer;
  }
}
