#include "unit_tests.h"

void polish_notation_correct_test(char* line, const char* correct[],
                                  const int size) {
  int err = 0;

  list* parsed = parse_line(line, &err);
  ck_assert_int_eq(err, 0);

  list* polished = parse_to_polish(parsed, &err);
  ck_assert_int_eq(err, 0);

  list* curr = polished;
  for (int i = 0; i < size && curr != NULL; ++i, curr = curr->next) {
    ck_assert_str_eq(correct[i], curr->lexem);
  }

  list_destroy_with_lexem(parsed);
  list_destroy(polished);
}

void calculation(char* line, double input, double expected) {
  int err = 0;

  list* parsed = parse_line(line, &err);
  ck_assert_int_eq(err, 0);

  list* polished = parse_to_polish(parsed, &err);
  ck_assert_int_eq(err, 0);

  double res = apply_polish(input, polished, &err);
  ck_assert_int_eq(err, 0);
  ck_assert_double_eq_tol(res, expected, EPS);

  list_destroy_with_lexem(parsed);
  list_destroy(polished);
}

void erroneous_pure_calculation(char* line) {
  int err = 0;

  list* parsed = parse_line(line, &err);
  ck_assert_int_eq(err, 0);

  list* polished = parse_to_polish(parsed, &err);
  ck_assert_int_eq(err, 0);

  apply_polish(0, polished, &err);
  ck_assert_int_eq(err, ERR);

  list_destroy_with_lexem(parsed);
  list_destroy(polished);
}
