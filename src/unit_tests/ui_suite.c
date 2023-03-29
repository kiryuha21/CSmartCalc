#include "unit_tests.h"

Suite* get_ui_suite() {
    Suite* s = suite_create("ui_suite");

    TCase* ui_suite = tcase_create("ui_suite");
    // tcase_add_test(ui_suite, pure_calculation_1);

    suite_add_tcase(s, ui_suite);

    return s;
}
