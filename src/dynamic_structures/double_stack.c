#include <stdlib.h>

#include "dynamic_structures.h"

double_stack* double_stack_push(double_stack* head, double num) {
  double_stack* temp = (double_stack*)calloc(1, sizeof(double_stack));
  if (temp != NULL) {
    temp->num = num;
    temp->next = head;
  } else {
    temp = head;
  }
  return temp;
}

double_stack* double_stack_pop(double_stack* head) {
  double_stack* temp = head;
  head = head->next;
  free(temp);
  return head;
}

void destroy_double_stack(double_stack* head) {
  while (head != NULL) {
    double_stack* temp = head;
    head = head->next;
    free(temp);
  }
}
