#include "../s21_smart_calc.h"

void on_input_focus(GtkWidget* w) { gtk_entry_set_text(GTK_ENTRY(w), ""); }

void evaluate_expression(EvaluationComponents* components) {
  GtkEntry* entry = GTK_ENTRY(components->expression_input);
  GtkTextView* view = GTK_TEXT_VIEW(components->result_view);
  GtkTextBuffer* buffer = gtk_text_view_get_buffer(view);

  int err = 0;
  gchar* expression = (gchar*)gtk_entry_get_text(entry);
  list* lexems = parse_line(expression, &err);
  if (err == 0) {
    list* parsed = parse_to_polish(lexems, &err);
    if (err == 0) {
      char buff[100] = {0};
      while (parsed != NULL) {
        gchar* text = parsed->lexem;
        strcat(buff, text);
        parsed = parsed->next;
      }
      gtk_text_buffer_set_text(buffer, buff, (int)strlen(buff));
      gtk_text_view_set_buffer(GTK_TEXT_VIEW(components->result_view), buffer);
    } else {
      gchar* text = "Polish notation error!";
      gtk_text_buffer_set_text(buffer, text, (int)strlen(text));
      gtk_text_view_set_buffer(GTK_TEXT_VIEW(components->result_view), buffer);
    }
  } else {
    gchar* text = "Parsing error!";
    gtk_text_buffer_set_text(buffer, text, (int)strlen(text));
    gtk_text_view_set_buffer(GTK_TEXT_VIEW(components->result_view), buffer);
  }

  gtk_widget_grab_focus(components->result_view);
}

gboolean on_key_press(GtkWidget* widget, GdkEventKey* event,
                      gpointer user_data) {
  switch (event->keyval) {
    case GDK_KEY_equal:
      printf("=         was pressed on %s", gtk_widget_get_name(widget));

      EvaluationComponents* components = user_data;
      evaluate_expression(components);

      break;
    default:
      printf("%-10s was pressed on %s\n", gdk_keyval_name(event->keyval),
             gtk_widget_get_name(widget));
      break;
  }
  return FALSE;
}
