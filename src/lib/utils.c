#include <ctype.h>

#include "../s21_smart_calc.h"

int is_empty(const char* string) {
  int result = 0;
  if (string == NULL || *string == '\0') {
    result = 1;
  } else {
    for (; *string != '\0'; ++string) {
      if (isspace(*string)) {
        result = 1;
      }
    }
  }
  return result;
}
