#include <stdlib.h>

#include "dynamic_structures.h"

string_stack* stack_push(struct string_stack* head, char* lexem) {
  string_stack* temp = (string_stack*)calloc(1, sizeof(string_stack));
  if (temp != NULL) {
    temp->lexem = lexem;
    temp->next = head;
  } else {
    temp = head;
  }
  return temp;
}

string_stack* pop(string_stack* head) {
  string_stack* temp = head;
  head = head->next;
  free(temp);
  return head;
}
