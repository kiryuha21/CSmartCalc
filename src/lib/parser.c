#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "../s21_smart_calc.h"

int is_solo_char(char line) {
  return line == '+' || line == '-' || line == '/' || line == '*' ||
         line == '(' || line == ')' || tolower(line) == 'x' || line == '^';
}

int has_prefix(const char* str, char* prefix) {
  int result = 1;
  if (strlen(prefix) > strlen(str)) {
    result = 0;
  } else {
    for (; *prefix != '\0'; ++str, ++prefix) {
      if (*str != *prefix) {
        result = 0;
      }
    }
  }
  return result;
}

int handle_number(char* line, list** result, list** current, int* err) {
  int capacity = 1;
  int size = 0;

  char* number = (char*)calloc(capacity, sizeof(char));
  if (number == NULL) {
    *err = ERR;
  } else {
    int points_count = 0;
    char* temp = line;
    while (*err == 0 && (isdigit(*temp) || *temp == '.')) {
      if (*temp == '.') {
        if (points_count == 0) {
          ++points_count;
        } else {
          *err = ERR;
        }
      }
      number[size] = *temp;
      size += 1;
      if (size == capacity) {
        capacity *= 2;
        char* new_mem = (char*)realloc(number, capacity);
        if (new_mem == NULL) {
          *err = ERR;
        } else {
          number = new_mem;
        }
      }
      ++temp;
    }
  }
  if (*err == ERR) {
    free(number);
  } else {
    number[size] = '\0';
    handle_push(result, current, number);
  }

  return size;
}

void allocate_and_push(int size, list** result, list** current, char* position,
                       int* err) {
  char* res = (char*)calloc(size + 1, sizeof(char));
  if (res == NULL) {
    *err = ERR;
  } else {
    strncpy(res, position, size);
    res[size] = '\0';
    handle_push(result, current, res);
  }
}

void push_unary_minus(list** result, list** current, int* err) {
  char* tilda = (char*)calloc(2, sizeof(char));
  if (tilda == NULL) {
    *err = ERR;
  } else {
    tilda[0] = '~';
    handle_push(result, current, tilda);
  }
}

int is_unary(list** current) {
  return *current == NULL || strcmp((*current)->lexem, "(") == 0;
}

char* handle_lexem(char* position, list** result, list** current, int* err) {
  if (is_solo_char(*position)) {
    if (*position == '-' && is_unary(current)) {
      push_unary_minus(result, current, err);
    } else if (*position != '+' || !is_unary(current)) {
      allocate_and_push(1, result, current, position, err);
    }
    ++position;
  } else if (has_prefix(position, "sin") || has_prefix(position, "cos") ||
             has_prefix(position, "tan") || has_prefix(position, "log") ||
             has_prefix(position, "mod")) {
    allocate_and_push(3, result, current, position, err);
    position += 3;
  } else if (has_prefix(position, "sqrt") || has_prefix(position, "asin") ||
             has_prefix(position, "acos") || has_prefix(position, "atan")) {
    allocate_and_push(4, result, current, position, err);
    position += 4;
  } else if (has_prefix(position, "ln")) {
    allocate_and_push(2, result, current, position, err);
    position += 2;
  } else if (isdigit(*position)) {
    int count = handle_number(position, result, current, err);
    position += count;
  } else {
    *err = ERR;
  }
  return position;
}

list* parse_line(char* line, int* err) {
  list* result = NULL;
  list* current = NULL;
  while (*err == 0 && *line != '\0') {
    if (*line == ' ') {
      ++line;
    } else {
      line = handle_lexem(line, &result, &current, err);
    }
  }
  return result;
}
