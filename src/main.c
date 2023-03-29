#include "s21_smart_calc.h"

int main(int argc, char* argv[]) {
  gtk_init(&argc, &argv);
  GError* error = NULL;

  GtkBuilder* builder = gtk_builder_new();
  if (gtk_builder_add_from_file(builder, "interface.ui", &error) == 0) {
    g_printerr("Error loading file: %s\n", error->message);
    g_clear_error(&error);
    return 1;
  }

  GtkCssProvider* cssProvider = gtk_css_provider_new();
  gtk_css_provider_load_from_path(cssProvider, "styles.css", NULL);
  gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                            GTK_STYLE_PROVIDER(cssProvider),
                                            GTK_STYLE_PROVIDER_PRIORITY_USER);

  /* Connect signal handlers to the constructed widgets. */
  GObject* main_window = gtk_builder_get_object(builder, "main_window");
  g_signal_connect(main_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  GObject* output = gtk_builder_get_object(builder, "result_view");
  GObject* input = gtk_builder_get_object(builder, "expression_input");

  EvaluationComponents components = {GTK_WIDGET(input), GTK_WIDGET(output)};
  g_signal_connect(input, "key_press_event", G_CALLBACK(on_key_press),
                   &components);
  g_signal_connect(input, "grab_focus", G_CALLBACK(on_input_focus), NULL);

  gtk_main();

  return 0;
}