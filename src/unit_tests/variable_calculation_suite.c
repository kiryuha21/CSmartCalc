#include "unit_tests.h"

Suite* get_variable_calculation_suite() {
  Suite* s = suite_create("variable_calculation_suite");

  TCase* variable_calculation_suite =
      tcase_create("variable_calculation_suite");
  // tcase_add_test(variable_calculation_suite, parse_unaries);

  suite_add_tcase(s, variable_calculation_suite);

  return s;
}