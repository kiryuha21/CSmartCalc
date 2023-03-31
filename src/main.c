#include "s21_smart_calc.h"

int main(int argc, char* argv[]) {
  setlocale(LC_NUMERIC, "ru_RU.UTF-8");
  gtk_init(&argc, &argv);
  GError* error = NULL;

  GtkBuilder* builder = gtk_builder_new();
  if (gtk_builder_add_from_file(builder, MAIN_UI_FILE, &error) == 0) {
    g_printerr("Error loading file: %s\n", error->message);
    g_clear_error(&error);
    return 1;
  }

  GtkCssProvider* cssProvider = gtk_css_provider_new();
  gtk_css_provider_load_from_path(cssProvider, STYLES_FILE, NULL);
  gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                            GTK_STYLE_PROVIDER(cssProvider),
                                            GTK_STYLE_PROVIDER_PRIORITY_USER);

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
  g_signal_connect(graph_button, "clicked", G_CALLBACK(plot_main), exp_input);

  gtk_main();

  g_ptr_array_free(arguments_array, gtk_false());

  return 0;
}