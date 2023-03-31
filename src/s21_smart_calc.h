#ifndef C7_SMARTCALC_V1_0_1_S21_SMART_CALC_H
#define C7_SMARTCALC_V1_0_1_S21_SMART_CALC_H

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>
#include <locale.h>

#include "dynamic_structures/dynamic_structures.h"

#ifndef M_PI_2
#define M_PI_2 1.57079632679489661923
#endif

#define UNUSED(x) (void)(x)

#define ERR (-1)

#define LOW_P 1
#define HIGH_P 2
#define HIGHEST_P 3

#define STYLES_FILE "graphics/styles.css"
#define PLOT_FILE "graphics/plot.png"
#define MAIN_UI_FILE "graphics/interface.ui"
#define PLOT_UI_FILE "graphics/graph.ui"

void evaluate_expression(GPtrArray* arguments);
gboolean on_key_press(GtkWidget* widget, GdkEventKey* event,
                      gpointer user_data);

void plot_main(GtkWidget* widget, gpointer data);

list* parse_line(char* line, int* err);
list* parse_to_polish(list* lexems, int* err);
double apply_polish(double x, list* parsed_polish, int* err);

int is_empty(const char* string);
int safe_get_double_from_str(const char* str, double* val);
void safe_solo_char_replace(char* str, char sym, char replacer);

#endif  // C7_SMARTCALC_V1_0_1_S21_SMART_CALC_H
