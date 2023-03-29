#include "unit_tests.h"

START_TEST(pure_calculation_1) { calculation("1+2/2^2", 0, 1 + 2 / pow(2, 2)); }

START_TEST(pure_calculation_2) {
  calculation("-sin(10^2 * cos(30))", 0, -sin(pow(10, 2) * cos(30)));
}

START_TEST(variable_calculation_1) {
  calculation("1+x/x^x", 2, 1 + 2 / pow(2, 2));
}

START_TEST(variable_calculation_2) {
  calculation("-sin(10^2 * cos(x))", 30, -sin(pow(10, 2) * cos(30)));
}

Suite* get_calculation_suite() {
  Suite* s = suite_create("pure_calculation_suite");

  TCase* calculation_suite = tcase_create("calculation_suite");
  tcase_add_test(calculation_suite, pure_calculation_1);
  tcase_add_test(calculation_suite, pure_calculation_2);
  tcase_add_test(calculation_suite, variable_calculation_1);
  tcase_add_test(calculation_suite, variable_calculation_2);

  suite_add_tcase(s, calculation_suite);

  return s;
}