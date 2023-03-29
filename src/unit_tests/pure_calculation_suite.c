#include "unit_tests.h"

Suite* get_pure_calculation_suite() {
  Suite* s = suite_create("pure_calculation_suite");

  TCase* pure_calculation_suite = tcase_create("pure_calculation_suite");
  // tcase_add_test(pure_calculation_suite, parse_unaries);

  suite_add_tcase(s, pure_calculation_suite);

  return s;
}