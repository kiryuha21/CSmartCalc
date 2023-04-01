#include <math.h>

#include "../s21_smart_calc.h"

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

char *validate_range(const gchar *sfirst, const gchar *ssecond, double *first,
                     double *second) {
  char *msg = NULL;
  if (safe_get_double_from_str(sfirst, first) != 0 ||
      safe_get_double_from_str(ssecond, second) != 0) {
    msg = INVALID_NUMBER_MSG;
  } else if (*second - *first <= 0) {
    msg = INVALID_LIMITS_MSG;
  }
  return msg;
}

char *generate_datafile(gchar *function, double min_x, double max_x) {
  int err = 0;
  char *err_msg = NULL;
  double diff = max_x - min_x;
  list *parsed = parse_line(function, &err);
  if (err == 0) {
    list *polished = parse_to_polish(parsed, &err);
    if (err == 0) {
      FILE *points = fopen(POINTS_FILE, "w");
      if (points != NULL) {
        double delta = diff / POINTS_AMOUNT;
        for (int i = 0; i < POINTS_AMOUNT && !err; ++i) {
          double x = min_x + delta * i;
          double y = apply_polish(x, polished, &err);
          if (err == 0 && !isnan(y) && !isinf(y)) {
            fprintf(points, "%f %f\n", x, y);
          }
        }
        fclose(points);
      } else {
        err_msg = FILE_ERROR_MSG;
      }
    } else {
      err_msg = NOTATION_ERROR_MSG;
    }
    list_destroy(polished);
  } else {
    err_msg = PARSING_ERROR_MSG;
  }
  list_destroy_with_lexem(parsed);
  return err_msg;
}

void plot_data(GtkWidget *button, gpointer user_data) {
  UNUSED(button);

  GPtrArray *array = user_data;
  gchar *function = array->pdata[0];
  const gchar *min_x = gtk_entry_get_text(GTK_ENTRY(array->pdata[1]));
  const gchar *max_x = gtk_entry_get_text(GTK_ENTRY(array->pdata[2]));
  const gchar *min_y = gtk_entry_get_text(GTK_ENTRY(array->pdata[3]));
  const gchar *max_y = gtk_entry_get_text(GTK_ENTRY(array->pdata[4]));
  GtkLabel *label = GTK_LABEL(array->pdata[5]);
  GtkWidget *plot_image = GTK_WIDGET(array->pdata[6]);

  if (is_empty(min_x) || is_empty(max_x) || is_empty(min_y) ||
      is_empty(max_y)) {
    gtk_label_set_label(label, MISSING_FIELDS_MSG);
    return;
  }

  double left_x = 0, right_x = 0, lower_y = 0, upper_y = 0;
  char *err_msg = validate_range(min_x, max_x, &left_x, &right_x);
  if (err_msg != NULL) {
    gtk_label_set_label(label, err_msg);
    return;
  }

  err_msg = validate_range(min_y, max_y, &lower_y, &upper_y);
  if (err_msg != NULL) {
    gtk_label_set_label(label, err_msg);
    return;
  }

  err_msg = generate_datafile(function, left_x, right_x);
  if (err_msg != NULL) {
    gtk_label_set_label(label, err_msg);
    return;
  }

  gint width = gtk_widget_get_allocated_width(plot_image);
  gint height = gtk_widget_get_allocated_height(plot_image);

  gchar *cmd = g_strdup("/usr/bin/gnuplot");
  gchar *script = g_strdup_printf(
      "set terminal pngcairo size %d,%d\n"
      "set output '%s'\n"
      "set xlabel \"X-axis\"\n"
      "set ylabel \"Y-axis\"\n"
      "set xrange [%s:%s]\n"
      "set yrange [%s:%s]\n"
      "set decimalsign locale\n"
      "plot \"%s\" title \"%s with scale %f\" ps 0.7",
      width, height, PLOT_PNG_FILE, min_x, max_x, min_y, max_y, POINTS_FILE,
      function, (upper_y - lower_y) / (right_x - left_x));
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
  GObject *min_y_entry = gtk_builder_get_object(builder, "min_y_entry");
  GObject *max_y_entry = gtk_builder_get_object(builder, "max_y_entry");
  GObject *graph_label = gtk_builder_get_object(builder, "graph_label");
  GObject *plot_image = gtk_builder_get_object(builder, "graph_image");

  GPtrArray *arguments_array = g_ptr_array_new();
  g_ptr_array_add(arguments_array, function);
  g_ptr_array_add(arguments_array, min_x_entry);
  g_ptr_array_add(arguments_array, max_x_entry);
  g_ptr_array_add(arguments_array, min_y_entry);
  g_ptr_array_add(arguments_array, max_y_entry);
  g_ptr_array_add(arguments_array, graph_label);
  g_ptr_array_add(arguments_array, plot_image);

  GObject *graph_window = gtk_builder_get_object(builder, "graph_window");
  g_signal_connect(graph_window, "destroy", G_CALLBACK(gtk_window_close), NULL);
  g_signal_connect(graph_window, "destroy", G_CALLBACK(free_array_on_close),
                   arguments_array);

  GObject *plot_button = gtk_builder_get_object(builder, "plot_button");
  g_signal_connect(plot_button, "clicked", G_CALLBACK(plot_data),
                   arguments_array);
}
