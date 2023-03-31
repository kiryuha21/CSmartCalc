#include "s21_smart_calc.h"

int main(int argc, char* argv[]) {
  setlocale(LC_NUMERIC, "ru_RU.UTF-8");
  gtk_init(&argc, &argv);

  GtkBuilder* builder = NULL;
  GtkCssProvider* provider = NULL;
  if (create_styled_window(&builder, MAIN_UI_FILE, &provider) == ERR) {
    return 0;
  }

  /* Connect signal handlers to the constructed widgets. */
  GObject* main_window = gtk_builder_get_object(builder, "main_window");
  g_signal_connect(main_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  GObject* output = gtk_builder_get_object(builder, "result_view");
  GObject* exp_input = gtk_builder_get_object(builder, "expression_input");
  GObject* var_input = gtk_builder_get_object(builder, "variable_input");

  GPtrArray* arguments_array = g_ptr_array_new();
  g_ptr_array_add(arguments_array, exp_input);
  g_ptr_array_add(arguments_array, var_input);
  g_ptr_array_add(arguments_array, output);

  g_signal_connect(exp_input, "key_press_event", G_CALLBACK(on_key_press),
                   arguments_array);

  GObject* graph_button = gtk_builder_get_object(builder, "graph_button");
  g_signal_connect(graph_button, "clicked", G_CALLBACK(create_graph_plotter),
                   exp_input);

  GObject* credit_calc_button =
      gtk_builder_get_object(builder, "credit_calc_button");
  g_signal_connect(credit_calc_button, "clicked",
                   G_CALLBACK(create_credit_calculator), NULL);

  GObject* deposit_calc_button =
      gtk_builder_get_object(builder, "deposit_calc_button");
  g_signal_connect(deposit_calc_button, "clicked",
                   G_CALLBACK(create_deposit_calculator), NULL);

  gtk_main();

  g_ptr_array_free(arguments_array, gtk_false());

  return 0;
}