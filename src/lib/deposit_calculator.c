#include "../s21_smart_calc.h"

void create_deposit_calculator(GtkWidget *widget, gpointer data) {
  UNUSED(widget);
  UNUSED(data);

  GtkBuilder *builder = NULL;
  GtkCssProvider *provider = NULL;
  if (create_styled_window(&builder, DEPOSIT_UI_FILE, &provider) == ERR) {
    return;
  }

  GObject *graph_window = gtk_builder_get_object(builder, "deposit_window");
  g_signal_connect(graph_window, "destroy", G_CALLBACK(gtk_window_close), NULL);
}
