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

#define INVALID_EXPRESSION_MSG "Invalid expression!"
#define INVALID_VARIABLE_MSG "Wrong or missing variable!"
#define INVALID_NUMBER_MSG "Invalid number!"
#define INVALID_LIMITS_MSG "Invalid range limits!"
#define NOTATION_ERROR_MSG "Polish notation error!"
#define PARSING_ERROR_MSG "Parsing error!"
#define FILE_ERROR_MSG "File opening error!"
#define MISSING_FIELDS_MSG "Missing fields!"

#define POINTS_AMOUNT 1000
#define POINTS_FILE "points.txt"

#define STYLES_FILE "graphics/styles.css"

#define PLOT_PNG_FILE "graphics/plot.png"

#define MAIN_UI_FILE "graphics/interface.ui"
#define PLOT_UI_FILE "graphics/graph.ui"
#define DEPOSIT_UI_FILE "graphics/deposit.ui"
#define CREDIT_UI_FILE "graphics/credit.ui"

void evaluate_expression(GPtrArray* arguments);
gboolean on_key_press(GtkWidget* widget, GdkEventKey* event,
                      gpointer user_data);

void create_graph_plotter(GtkWidget* widget, gpointer data);
void create_credit_calculator(GtkWidget* widget, gpointer data);
void create_deposit_calculator(GtkWidget* widget, gpointer data);

list* parse_line(char* line, int* err);
list* parse_to_polish(list* lexems, int* err);
double apply_polish(double x, list* parsed_polish, int* err);

int is_empty(const char* string);
int safe_get_double_from_str(const char* str, double* val);
void safe_solo_char_replace(char* str, char sym, char replacer);
int create_styled_window(GtkBuilder** builder, const char* ui_filename,
                         GtkCssProvider** provider);

#endif  // C7_SMARTCALC_V1_0_1_S21_SMART_CALC_H
