#ifndef C7_SMARTCALC_V1_0_1_S21_SMART_CALC_H
#define C7_SMARTCALC_V1_0_1_S21_SMART_CALC_H

#include "dynamic_structures/dynamic_structures.h"

#ifndef M_PI_2
#define M_PI_2 1.57079632679489661923
#endif

#define ERR (-1)

#define LOW_P 1
#define HIGH_P 2

list* parse_line(char* line, int* err);
list* parse_to_polish(list* lexems, int* err);
double apply_polish(double x, list* parsed_polish, int* err);

#endif  // C7_SMARTCALC_V1_0_1_S21_SMART_CALC_H
