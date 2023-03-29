#include <stdlib.h>
#include <string.h>

#include "dynamic_structures.h"

list* list_init(char* lexem) {
  list* head = NULL;

  head = (list*)calloc(1, sizeof(list));
  if (head != NULL) {
    head->lexem = lexem;
    head->next = NULL;
  }

  return head;
}

list* add_element(struct list* elem, char* lexem) {
  list* return_value = NULL;

  list* temp = elem->next;
  elem->next = (list*)calloc(1, sizeof(list));
  if (elem->next != NULL) {  // if element was allocated
    elem->next->next = temp;
    elem->next->lexem = lexem;
    return_value = elem->next;
  } else {  // restore state if not
    elem->next = temp;
  }

  return return_value;
}

void handle_push(list** head, list** current, char* lexem) {
  *current = list_push(lexem, *current);
  if (*head == NULL) {
    *head = *current;
  }
}

list* list_push(char* lexem, list* tail) {
  list* return_val = NULL;
  if (tail == NULL) {
    return_val = list_init(lexem);
  } else {
    return_val = add_element(tail, lexem);
  }
  return return_val;
}

void list_destroy(list* root) {
  while (root != NULL) {
    list* temp = root;
    root = root->next;
    free(temp);
  }
}

void list_destroy_with_lexem(list* root) {
  while (root != NULL) {
    list* temp = root;
    root = root->next;
    free(temp->lexem);
    free(temp);
  }
}

list* list_find(list* root, const char* value) {
  list* result = NULL;
  for (list* curr = root; curr != NULL; curr = curr->next) {
    if (strcmp(curr->lexem, value) == 0) {
      result = curr;
    }
  }
  return result;
}
