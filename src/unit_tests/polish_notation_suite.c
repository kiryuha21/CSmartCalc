#include "unit_tests.h"

Suite* get_polish_notation_suite() {
  Suite* s = suite_create("polish_notation_suite");

  TCase* polish_notation_cases = tcase_create("polish_notation_cases");
  // tcase_add_test(polish_notation_cases, parse_unaries);

  suite_add_tcase(s, polish_notation_cases);

  return s;
}
