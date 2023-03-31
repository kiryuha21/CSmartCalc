#ifndef C7_SMARTCALC_V1_0_1_UNIT_TESTS_H
#define C7_SMARTCALC_V1_0_1_UNIT_TESTS_H

#include <check.h>

#include "../s21_smart_calc.h"

#define SUITES_COUNT 3

#define EPS 1e-7

Suite* get_parsing_suite();
Suite* get_polish_notation_suite();
Suite* get_calculation_suite();

void polish_notation_correct_test(char* line, const char* correct[], int size);
void calculation(char* line, double input, double expected);
void erroneous_pure_calculation(char* line);

#endif  // C7_SMARTCALC_V1_0_1_UNIT_TESTS_H
