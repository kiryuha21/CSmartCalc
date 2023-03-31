#include "../s21_smart_calc.h"

void free_array(GtkWidget *window, gpointer user_data) {
  GPtrArray *array = user_data;
  g_ptr_array_free(array, gtk_false());
  printf("%s freed array", gtk_widget_get_name(window));
}

gboolean read_err(gpointer user_data) {
  GObject *pollable_stream = user_data;

  g_print("Read Stream\n");
  char *buffer = g_malloc(500);
  memset(buffer, '\0', 500);
  gssize bytes_read = 0;

  bytes_read = g_pollable_input_stream_read_nonblocking(
      (GPollableInputStream *)pollable_stream, buffer, 499, NULL, NULL);
  if (bytes_read > 0) {
    g_print("Error From Gnuplot: %s", buffer);
  }
  if (bytes_read == -1) {
    g_print("Input Stream Error.");
  }
  g_input_stream_close((GInputStream *)pollable_stream, NULL, NULL);
  g_free(buffer);

  return FALSE;
}

void gnuplot_finished(GObject *source_object, GAsyncResult *res,
                      GtkWidget *image) {
  UNUSED(res);

  gtk_image_set_from_file(GTK_IMAGE(image), PLOT_PNG_FILE);
  gtk_widget_queue_draw(image);
  if (g_subprocess_get_successful(G_SUBPROCESS(source_object))) {
    g_print("Gnuplot Success\n");
  }
  g_object_unref(source_object);
  g_print("Gnuplot Finished\n");
}

GPtrArray *get_str_x_limits(const char *raw_x_min, const char *raw_x_max) {
  GPtrArray *res = g_ptr_array_new();

  double x_min = 0, x_max = 0;
  char *x_min_str = NULL;
  char *x_max_str = NULL;
  if (!is_empty(raw_x_min)) {
    safe_get_double_from_str(raw_x_min, &x_min);
    x_min_str = calloc(100, sizeof(char));
    sprintf(x_min_str, "%.7f", x_min);
    safe_solo_char_replace(x_min_str, ',', '.');
  }
  if (!is_empty(raw_x_max)) {
    safe_get_double_from_str(raw_x_max, &x_max);
    x_max_str = calloc(100, sizeof(char));
    sprintf(x_max_str, "%.7f", x_max);
    safe_solo_char_replace(x_max_str, ',', '.');
  }

  g_ptr_array_add(res, x_min_str);
  g_ptr_array_add(res, x_max_str);

  return res;
}

void call_gnuplot(gchar *cmd, gchar *script, GtkWidget *plot_image) {
  GSubprocess *sub_process = g_subprocess_new(
      G_SUBPROCESS_FLAGS_STDIN_PIPE | G_SUBPROCESS_FLAGS_STDERR_PIPE, NULL, cmd,
      NULL);
  GInputStream *err_stream = g_subprocess_get_stderr_pipe(sub_process);
  GSource *source = g_pollable_input_stream_create_source(
      (GPollableInputStream *)err_stream, NULL);
  g_source_attach(source, NULL);
  g_source_set_callback(source, (GSourceFunc)read_err, NULL, NULL);
  g_subprocess_wait_async(sub_process, NULL,
                          (GAsyncReadyCallback)gnuplot_finished, plot_image);
  GOutputStream *stream = g_subprocess_get_stdin_pipe(sub_process);
  g_output_stream_write(stream, script, strlen(script) + 1, NULL, NULL);
  g_output_stream_flush(stream, NULL, NULL);
  g_output_stream_close(stream, NULL, NULL);
}

void plot_data(GtkWidget *button, gpointer user_data) {
  UNUSED(button);

  GPtrArray *array = user_data;
  gchar *function = array->pdata[0];
  GtkEntry *min_x_entry = GTK_ENTRY(array->pdata[1]);
  GtkEntry *max_x_entry = GTK_ENTRY(array->pdata[2]);
  GtkWidget *plot_image = GTK_WIDGET(array->pdata[3]);

  const char *min_entry_text = gtk_entry_get_text(min_x_entry);
  const char *max_entry_text = gtk_entry_get_text(max_x_entry);
  GPtrArray *limits = get_str_x_limits(min_entry_text, max_entry_text);

  gint width = gtk_widget_get_allocated_width(plot_image);
  gint height = gtk_widget_get_allocated_height(plot_image);

  gchar *cmd = g_strdup("/usr/bin/gnuplot");
  gchar *script = g_strdup_printf(
      "set terminal pngcairo size %d,%d\n"
      "set output '%s'\n"
      "set xlabel \"X-axis\"\n"
      "set ylabel \"Y-axis\"\n"
      "set xrange [%s:%s]\n"
      "plot %s",
      width, height, PLOT_PNG_FILE,
      limits->pdata[0] ? (char *)limits->pdata[0] : "",
      limits->pdata[1] ? (char *)limits->pdata[1] : "", function);
  g_ptr_array_free(limits, gtk_true());
  g_print("%s\n", script);

  call_gnuplot(cmd, script, plot_image);

  g_free(cmd);
  g_free(script);
}

void create_graph_plotter(GtkWidget *widget, gpointer data) {
  UNUSED(widget);

  GtkBuilder *builder = NULL;
  GtkCssProvider *provider = NULL;
  if (create_styled_window(&builder, PLOT_UI_FILE, &provider) == ERR) {
    return;
  }

  gchar *function = (gchar *)gtk_entry_get_text(GTK_ENTRY(data));
  GObject *min_x_entry = gtk_builder_get_object(builder, "min_x_entry");
  GObject *max_x_entry = gtk_builder_get_object(builder, "max_x_entry");
  GObject *plot_image = gtk_builder_get_object(builder, "graph_image");

  GPtrArray *arguments_array = g_ptr_array_new();
  g_ptr_array_add(arguments_array, function);
  g_ptr_array_add(arguments_array, min_x_entry);
  g_ptr_array_add(arguments_array, max_x_entry);
  g_ptr_array_add(arguments_array, plot_image);

  GObject *graph_window = gtk_builder_get_object(builder, "graph_window");
  g_signal_connect(graph_window, "destroy", G_CALLBACK(gtk_window_close), NULL);
  g_signal_connect(graph_window, "destroy", G_CALLBACK(free_array),
                   arguments_array);

  GObject *plot_button = gtk_builder_get_object(builder, "plot_button");
  g_signal_connect(plot_button, "clicked", G_CALLBACK(plot_data),
                   arguments_array);
}
