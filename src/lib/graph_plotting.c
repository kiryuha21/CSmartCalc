#include "../s21_smart_calc.h"

gboolean read_err(GObject *pollable_stream, gpointer user_data) {
  g_print("Read Stream\n");
  char *buffer = g_malloc(500);
  memset(buffer, '\0', 500);
  gssize bytes_read = 0;

  bytes_read = g_pollable_input_stream_read_nonblocking(
      (GPollableInputStream *)pollable_stream, buffer, 499, NULL, NULL);
  if (bytes_read > 0) g_print("Error From Gnuplot: %s", buffer);
  if (bytes_read == -1) g_print("Input Stream Error.");
  g_input_stream_close((GInputStream *)pollable_stream, NULL, NULL);
  g_free(buffer);

  return FALSE;
}

void gnuplot_finished(GObject *source_object, GAsyncResult *res,
                      GtkWidget *image) {
  gtk_image_set_from_file(GTK_IMAGE(image), PLOT_FILE);
  gtk_widget_queue_draw(image);
  if (g_subprocess_get_successful(G_SUBPROCESS(source_object)))
    g_print("Gnuplot Success\n");
  g_object_unref(source_object);
  g_print("Gnuplot Finished\n");
}

void plot_data(GtkWidget *button, gpointer user_data) {
  PlottingComponents *components = user_data;
  GtkWidget *widget = GTK_WIDGET(components->plot_image);
  gint width =
      gtk_widget_get_allocated_width(GTK_WIDGET(components->plot_image));
  gint height =
      gtk_widget_get_allocated_height(GTK_WIDGET(components->plot_image));

  gchar *cmd = g_strdup("/usr/bin/gnuplot");
  gchar *script = g_strdup_printf(
      "set terminal pngcairo size %d,%d\n"
      "set output '%s'\n"
      "set xlabel \"X-axis\"\n"
      "set ylabel \"Y-axis\"\n"
      "plot %s",
      width, height, PLOT_FILE, components->function);
  g_print("%s\n", script);

  // The Gnuplot process.
  GSubprocess *sub_process = g_subprocess_new(
      G_SUBPROCESS_FLAGS_STDIN_PIPE | G_SUBPROCESS_FLAGS_STDERR_PIPE, NULL, cmd,
      NULL);
  // Set up the stderr pipe and callback.
  GInputStream *err_stream = g_subprocess_get_stderr_pipe(sub_process);
  GSource *source = g_pollable_input_stream_create_source(
      (GPollableInputStream *)err_stream, NULL);
  g_source_attach(source, NULL);
  g_source_set_callback(source, (GSourceFunc)read_err, NULL, NULL);
  // Set up finished callback.
  g_subprocess_wait_async(sub_process, NULL,
                          (GAsyncReadyCallback)gnuplot_finished,
                          components->plot_image);
  // Set up the stdin pipe and send script to gnuplot.
  GOutputStream *stream = g_subprocess_get_stdin_pipe(sub_process);
  // Send the \0 in the string also.
  g_output_stream_write(stream, script, strlen(script) + 1, NULL, NULL);
  g_output_stream_flush(stream, NULL, NULL);
  g_output_stream_close(stream, NULL, NULL);

  g_free(cmd);
  g_free(script);
}

void main_plot(GtkWidget *widget, gpointer data) {
  GError *error = NULL;
  GtkBuilder *builder = gtk_builder_new();
  if (gtk_builder_add_from_file(builder, "graph.ui", &error) == 0) {
    g_printerr("Error loading file: %s\n", error->message);
    g_clear_error(&error);
    return;
  }

  GtkCssProvider *cssProvider = gtk_css_provider_new();
  gtk_css_provider_load_from_path(cssProvider, "styles.css", NULL);
  gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                            GTK_STYLE_PROVIDER(cssProvider),
                                            GTK_STYLE_PROVIDER_PRIORITY_USER);

  GObject *graph_window = gtk_builder_get_object(builder, "graph_window");
  g_signal_connect(graph_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  gchar *function = (gchar *)gtk_entry_get_text(GTK_ENTRY(data));
  GObject *min_x_entry = gtk_builder_get_object(builder, "min_x_entry");
  GObject *max_x_entry = gtk_builder_get_object(builder, "max_x_entry");
  GObject *plot_image = gtk_builder_get_object(builder, "graph_image");
  PlottingComponents components = {min_x_entry, max_x_entry, plot_image,
                                   function};

  GObject *plot_button = gtk_builder_get_object(builder, "plot_button");
  g_signal_connect(plot_button, "clicked", G_CALLBACK(plot_data), &components);
}
