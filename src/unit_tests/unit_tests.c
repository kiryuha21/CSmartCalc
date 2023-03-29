#include "unit_tests.h"

int main() {
  Suite* suites[SUITES_COUNT] = {get_parsing_suite(),
                                 get_polish_notation_suite(),
                                 get_calculation_suite()};

  for (int i = 0; i < SUITES_COUNT; ++i) {
    SRunner* srunner = srunner_create(suites[i]);
    srunner_run_all(srunner, CK_NORMAL);
    srunner_free(srunner);
  }

  return 0;
}
