#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "../s21_smart_calc.h"

int is_num(char* lexem) {
  int result = 1;
  if (strcmp(lexem, "x") != 0) {
    char* temp;
    for (temp = lexem; *temp != '\0' && result == 1; ++temp) {
      if (!((*temp >= '0' && *temp <= '9') || *temp == '.')) {
        result = 0;
      }
    }
  }
  return result;
}

int is_prefix(char* lexem) {
  return strcmp(lexem, "sin") == 0 || strcmp(lexem, "cos") == 0 ||
         strcmp(lexem, "tan") == 0 || strcmp(lexem, "ctg") == 0 ||
         strcmp(lexem, "sqrt") == 0 || strcmp(lexem, "ln") == 0 ||
         strcmp(lexem, "~") == 0;
}

int is_binary(char* lexem) {
  return strcmp(lexem, "+") == 0 || strcmp(lexem, "*") == 0 ||
         strcmp(lexem, "/") == 0 || strcmp(lexem, "-") == 0 ||
         strcmp(lexem, "^") == 0 || strcmp(lexem, "mod") == 0;
}

int get_priority(char* lexem) {
  int val = 0;
  if (strcmp(lexem, "+") == 0 || strcmp(lexem, "-") == 0) {
    val = LOW_P;
  } else if (strcmp(lexem, "*") == 0 || strcmp(lexem, "/") == 0 ||
             strcmp(lexem, "mod") == 0) {
    val = HIGH_P;
  } else if (strcmp(lexem, "^") == 0) {
    val = HIGHEST_P;
  }
  return val;
}

string_stack* handle_binary(string_stack* head, list** result, list** curr,
                            char* lexem) {
  int stop = 0;
  while (!stop) {
    if (head != NULL && (is_prefix(head->lexem) ||
                         (get_priority(head->lexem) >= get_priority(lexem)))) {
      handle_push(result, curr, head->lexem);
      head = pop(head);
    } else {
      stop = 1;
    }
  }
  head = stack_push(head, lexem);
  return head;
}

list* parse_to_polish(list* lexems, int* err) {
  list* result = NULL;
  list* current = NULL;

  string_stack* operations_stack = NULL;
  list* temp;
  for (temp = lexems; temp != NULL && *err == 0; temp = temp->next) {
    if (is_prefix(temp->lexem) || strcmp(temp->lexem, "(") == 0) {
      operations_stack = stack_push(operations_stack, temp->lexem);
    } else if (is_num(temp->lexem)) {
      handle_push(&result, &current, temp->lexem);
    } else if (strcmp(temp->lexem, ")") == 0) {
      while (operations_stack != NULL &&
             strcmp(operations_stack->lexem, "(") != 0) {
        handle_push(&result, &current, operations_stack->lexem);
        operations_stack = pop(operations_stack);
      }
      if (operations_stack == NULL) {
        *err = ERR;
      } else {
        operations_stack = pop(operations_stack);
      }
    } else {  // is binary
      operations_stack =
          handle_binary(operations_stack, &result, &current, temp->lexem);
    }
  }

  while (operations_stack != NULL) {
    handle_push(&result, &current, operations_stack->lexem);
    operations_stack = pop(operations_stack);
  }

  return result;
}

double apply_binary(double_stack** main_stack, char* lexem) {
  double result;

  double first = (*main_stack)->num;
  *main_stack = double_stack_pop(*main_stack);
  double second = (*main_stack)->num;
  *main_stack = double_stack_pop(*main_stack);

  if (strcmp(lexem, "-") == 0) {
    result = second - first;
  } else if (strcmp(lexem, "+") == 0) {
    result = second + first;
  } else if (strcmp(lexem, "/") == 0) {
    result = second / first;
  } else if (strcmp(lexem, "^") == 0) {
    result = pow(second, first);
  } else if (strcmp(lexem, "mod") == 0) {
    result = fmod(second, first);
  } else {  // lexem == *
    result = second * first;
  }

  return result;
}

double apply_prefix(double_stack** main_stack, char* lexem) {
  double result;

  double first = (*main_stack)->num;
  *main_stack = double_stack_pop(*main_stack);

  if (strcmp(lexem, "sin") == 0) {
    result = sin(first);
  } else if (strcmp(lexem, "cos") == 0) {
    result = cos(first);
  } else if (strcmp(lexem, "tan") == 0) {
    result = tan(first);
  } else if (strcmp(lexem, "ctg") == 0) {
    result = tan(M_PI_2 - first);
  } else if (strcmp(lexem, "sqrt") == 0) {
    result = sqrt(first);
  } else if (strcmp(lexem, "ln") == 0) {
    result = log(first);
  } else {  // lexem == "~"
    result = -first;
  }

  return result;
}

double apply_polish(double x, list* parsed_polish, int* err) {
  double_stack* main_stack = NULL;
  list* temp;
  double result = 0;
  for (temp = parsed_polish; *err == 0 && temp != NULL; temp = temp->next) {
    if (is_num(temp->lexem)) {
      if (strcmp(temp->lexem, "x") == 0) {
        result = x;
      } else {
        result = strtod(temp->lexem, NULL);
      }
    } else if (is_binary(temp->lexem)) {
      if (main_stack == NULL || main_stack->next == NULL) {
        *err = ERR;
      } else {
        result = apply_binary(&main_stack, temp->lexem);
      }
    } else {  // if prefix
      if (main_stack == NULL) {
        *err = ERR;
      } else {
        result = apply_prefix(&main_stack, temp->lexem);
      }
    }
    main_stack = double_stack_push(main_stack, result);
  }
  if (main_stack != NULL) {
    result = main_stack->num;
    main_stack = double_stack_pop(main_stack);
  } else {
    *err = ERR;
  }
  if (main_stack != NULL) {
    destroy_double_stack(main_stack);
    *err = ERR;
  }
  return result;
}
