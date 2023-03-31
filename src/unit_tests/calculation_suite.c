#include "unit_tests.h"

START_TEST(pure_calculation_1) { calculation("1+2/2^2", 0, 1.5); }

START_TEST(pure_calculation_2) {
  calculation("-sin(10^2 * cos(30))", 0, -sin(pow(10, 2) * cos(30)));
}

START_TEST(pure_calculation_3) { calculation("10 * 0.5 - 5.5", 0, -0.5); }

START_TEST(pure_calculation_4) {
  calculation("5 mod 12 - 5.5 mod 3.5", 0, 5 - fmod(5.5, 3.5));
}

START_TEST(variable_calculation_1) {
  calculation("1+x/x^x", 2, 1 + 2 / pow(2, 2));
}

START_TEST(variable_calculation_2) {
  calculation("-sin(10^2 * cos(x))", 30, -sin(pow(10, 2) * cos(30)));
}

START_TEST(variable_calculation_3) {
  calculation("sin(asin(x)) + cos(acos(x)) + tan(atan(x))", 1, 3);
}

START_TEST(variable_calculation_4) { calculation("sqrt(log(x)^2)", 100000, 5); }

START_TEST(variable_calculation_5) { calculation("ln x + log 10", exp(1), 2); }

START_TEST(calculation_with_error_1) {
  erroneous_pure_calculation("12 + 15 12");
}

START_TEST(calculation_with_error_2) { erroneous_pure_calculation("5+"); }

START_TEST(calculation_with_error_3) { erroneous_pure_calculation("sin()"); }

Suite* get_calculation_suite() {
  Suite* s = suite_create("calculation_suite");

  TCase* calculation_suite = tcase_create("calculation_suite");
  tcase_add_test(calculation_suite, pure_calculation_1);
  tcase_add_test(calculation_suite, pure_calculation_2);
  tcase_add_test(calculation_suite, pure_calculation_3);
  tcase_add_test(calculation_suite, pure_calculation_4);
  tcase_add_test(calculation_suite, variable_calculation_1);
  tcase_add_test(calculation_suite, variable_calculation_2);
  tcase_add_test(calculation_suite, variable_calculation_3);
  tcase_add_test(calculation_suite, variable_calculation_4);
  tcase_add_test(calculation_suite, variable_calculation_5);
  tcase_add_test(calculation_suite, calculation_with_error_1);
  tcase_add_test(calculation_suite, calculation_with_error_2);
  tcase_add_test(calculation_suite, calculation_with_error_3);

  suite_add_tcase(s, calculation_suite);

  return s;
}