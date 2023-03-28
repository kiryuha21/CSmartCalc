#ifndef C7_SMARTCALC_V1_0_1_DYNAMIC_STRUCTURES_H
#define C7_SMARTCALC_V1_0_1_DYNAMIC_STRUCTURES_H

typedef struct list {
  char* lexem;
  struct list* next;
} list;

list* list_init(char* lexem);
list* add_element(list* elem, char* lexem);
list* list_push(char* lexem, list* tail);
void list_destroy(list* root);
void extended_destroy(list* root);
void handle_push(list** head, list** current, char* lexem);

typedef struct string_stack {
  char* lexem;
  struct string_stack* next;
} string_stack;

string_stack* stack_push(string_stack* head, char* lexem);
string_stack* pop(string_stack* head);

typedef struct double_stack {
  double num;
  struct double_stack* next;
} double_stack;

double_stack* double_stack_push(double_stack* head, double num);
double_stack* double_stack_pop(double_stack* head);
void destroy_double_stack(double_stack* head);

#endif  // C7_SMARTCALC_V1_0_1_DYNAMIC_STRUCTURES_H
