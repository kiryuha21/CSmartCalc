#include "unit_tests.h"

START_TEST(parse_correct_unaries) {
  char* line = "sin(10) cos(15) -tan(20) ctg(-30) sqrt(12) ln(27)";

  int err = 0;
  list* queue = parse_line(line, &err);

  ck_assert_int_eq(err, 0);

  const int size = 26;
  char* correct_lexems[] = {"sin",  "(", "10", ")", "cos", "(", "15", ")",  "-",
                            "tan",  "(", "20", ")", "ctg", "(", "~",  "30", ")",
                            "sqrt", "(", "12", ")", "ln",  "(", "27", ")"};

  list* curr = queue;
  int i = 0;
  for (; curr != NULL && i < size; ++i, curr = curr->next) {
    ck_assert_str_eq(curr->lexem, correct_lexems[i]);
  }

  ck_assert_ptr_eq(curr, NULL);
  ck_assert_int_eq(i, size);

  list_destroy(queue);
}
END_TEST

START_TEST(parse_wrong_unaries) {
  char* line = "sin(10) cas(20)";

  int err = 0;
  list* queue = parse_line(line, &err);

  ck_assert_int_eq(err, ERR);

  list_destroy(queue);
}

START_TEST(parse_correct_numbers) {
  char* line = "12.1 * 13 / 14.15 + 1.0";

  int err = 0;
  list* queue = parse_line(line, &err);

  const int size = 7;
  char* correct_lexems[] = {"12.1", "*", "13", "/", "14.15", "+", "1.0"};

  list* curr = queue;
  int i = 0;
  for (; curr != NULL && i < size; ++i, curr = curr->next) {
    ck_assert_str_eq(curr->lexem, correct_lexems[i]);
  }

  ck_assert_ptr_eq(curr, NULL);
  ck_assert_int_eq(i, size);

  list_destroy(queue);
}

START_TEST(parse_wrong_numbers_1) {
  char* line = "12.12.1 + 5";

  int err = 0;
  list* queue = parse_line(line, &err);

  ck_assert_int_eq(err, ERR);

  list_destroy(queue);
}

START_TEST(parse_wrong_numbers_2) {
  char* line = "15.a1";

  int err = 0;
  list* queue = parse_line(line, &err);

  ck_assert_int_eq(err, ERR);

  list_destroy(queue);
}

Suite* get_parsing_suite() {
  Suite* s = suite_create("parsing_suite");

  TCase* parsing_cases = tcase_create("parsing_cases");
  tcase_add_test(parsing_cases, parse_correct_unaries);
  tcase_add_test(parsing_cases, parse_wrong_unaries);
  tcase_add_test(parsing_cases, parse_correct_numbers);
  tcase_add_test(parsing_cases, parse_wrong_numbers_1);
  tcase_add_test(parsing_cases, parse_wrong_numbers_2);

  suite_add_tcase(s, parsing_cases);

  return s;
}
