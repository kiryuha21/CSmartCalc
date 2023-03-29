#include "unit_tests.h"

START_TEST(wrong_polish) {
  char* line = "12 * 12 + 1)";
  int err = 0;

  list* parsed = parse_line(line, &err);
  ck_assert_int_eq(err, 0);

  list* polished = parse_to_polish(parsed, &err);
  ck_assert_int_eq(err, ERR);

  list_destroy_with_lexem(parsed);
  list_destroy(polished);
}

START_TEST(order_test_1) {
  const int size = 7;
  char* line = "12 + 12 / 1 ^ 5";
  const char* correct_polish[] = {"12", "12", "1", "5", "^", "/", "+"};

  polish_notation_correct_test(line, correct_polish, size);
}

START_TEST(order_test_2) {
  const int size = 7;
  char* line = "12^(1+1) + 3";
  const char* correct_polish[] = {"12", "1", "1", "+", "^", "3", "+"};

  polish_notation_correct_test(line, correct_polish, size);
}

START_TEST(order_test_3) {
  const int size = 8;
  char* line = "sin(cos(12 + log(5^3)))";
  const char* correct_polish[] = {"12",  "5", "3",   "^",
                                  "log", "+", "cos", "sin"};

  polish_notation_correct_test(line, correct_polish, size);
}

Suite* get_polish_notation_suite() {
  Suite* s = suite_create("polish_notation_suite");

  TCase* polish_notation_cases = tcase_create("polish_notation_cases");
  tcase_add_test(polish_notation_cases, wrong_polish);
  tcase_add_test(polish_notation_cases, order_test_1);
  tcase_add_test(polish_notation_cases, order_test_2);
  tcase_add_test(polish_notation_cases, order_test_3);

  suite_add_tcase(s, polish_notation_cases);

  return s;
}
