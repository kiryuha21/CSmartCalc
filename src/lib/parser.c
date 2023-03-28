#include <stdlib.h>
#include <string.h>

#include "../s21_smart_calc.h"

int is_solo_char(char line) {
  return line == '+' || line == '-' || line == '/' || line == '*' ||
         line == '(' || line == ')' || line == 'x';
}

int is_digit(char ch) { return ch >= '0' && ch <= '9'; }

int handle_number(char* line, list** result, list** current, int* err) {
  int capacity = 1;
  int size = 0;

  char* number = (char*)calloc(capacity, sizeof(char));
  if (number == NULL) {
    *err = ERR;
  } else {
    int points_count = 0;
    char* temp = line;
    while (*err == 0 && (is_digit(*temp) || *temp == '.')) {
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

void push_unary(list** result, list** current, int* err) {
  char* tilda = (char*)calloc(2, sizeof(char));
  if (tilda == NULL) {
    *err = ERR;
  } else {
    tilda[0] = '~';
    handle_push(result, current, tilda);
  }
}

char* handle_lexem(char* position, list** result, list** current, int* err) {
  if (is_solo_char(*position)) {
    if (*position == '-' &&
        ((*current) == NULL || strcmp((*current)->lexem, "(") == 0)) {
      push_unary(result, current, err);
    } else {
      allocate_and_push(1, result, current, position, err);
    }
    ++position;
  } else if ((*position == 's' && *(position + 1) == 'i' &&
              *(position + 2) == 'n') ||
             (*position == 'c' && *(position + 1) == 'o' &&
              *(position + 2) == 's') ||
             (*position == 't' && *(position + 1) == 'a' &&
              *(position + 2) == 'n') ||
             (*position == 'c' && *(position + 1) == 't' &&
              *(position + 2) == 'g')) {
    allocate_and_push(3, result, current, position, err);
    position += 3;
  } else if (*position == 's' && *(position + 1) == 'q' &&
             *(position + 2) == 'r' && *(position + 3) == 't') {
    allocate_and_push(4, result, current, position, err);
    position += 4;
  } else if (*position == 'l' && *(position + 1) == 'n') {
    allocate_and_push(2, result, current, position, err);
    position += 2;
  } else if (is_digit(*position)) {
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
