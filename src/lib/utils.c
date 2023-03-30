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

int safe_get_double_from_str(const char* str, double* val) {
  int return_code = 0;
  char* input_copy = calloc(strlen(str) + 1, sizeof(char));
  strcpy(input_copy, str);

  char* sep = strchr(input_copy, '.');
  if (sep != NULL) {
    *sep = ',';
  }

  char* endptr;
  *val = strtod(input_copy, &endptr);
  if (*endptr != '\0') {
    return_code = ERR;
  }
  free(input_copy);

  return return_code;
}

void safe_solo_char_replace(char* str, char sym, char replacer) {
  char* search = strchr(str, sym);
  if (search != NULL) {
    size_t pos = search - str;
    str[pos] = replacer;
  }
}
