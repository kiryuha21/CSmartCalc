#include "unit_tests.h"

START_TEST(normal_sin_datafile) {
  char* err = generate_datafile("sin(x)", -10, 10);
  ck_assert_ptr_null(err);

  FILE* file = fopen("points.txt", "r");
  ck_assert_ptr_ne(file, NULL);

  char* line = NULL;
  size_t len;
  while (getline(&line, &len, file) != -1) {
    double x = 0, y = 0;
    sscanf(line, "%lf %lf", &x, &y);
    ck_assert_double_eq_tol(sin(x), y, 1e-6);
  }

  fclose(file);
}

START_TEST(parse_error) {
  char* err = generate_datafile("1,1", -10, 10);
  ck_assert_str_eq(err, PARSING_ERROR_MSG);
}

START_TEST(polish_error) {
  char* err = generate_datafile("12*12+1)", -10, 10);
  ck_assert_str_eq(err, NOTATION_ERROR_MSG);
}

Suite* get_datafile_generation_suite() {
  Suite* s = suite_create("datafile_generation_suite");

  TCase* parsing_cases = tcase_create("datafile_generation_cases");
  tcase_add_test(parsing_cases, normal_sin_datafile);
  tcase_add_test(parsing_cases, polish_error);
  tcase_add_test(parsing_cases, parse_error);
  suite_add_tcase(s, parsing_cases);

  return s;
}
