#include "../s21_smart_calc.h"

int safe_get_input(GtkEntry* entry, list* lexems, double* input) {
  int return_code = 0;
  const gchar* str_value = gtk_entry_get_text(entry);
  if (is_empty(str_value)) {
    if (list_find(lexems, "x") != NULL) {
      return_code = ERR;
    }
    *input = 0;
  } else {
    return_code = safe_get_double_from_str(str_value, input);
  }
  return return_code;
}

void error_msg_to_buffer(GtkTextBuffer* buffer, char* text) {
  gtk_text_buffer_set_text(buffer, text, (int)strlen(text));
}

void evaluate_expression(GPtrArray* arguments) {
  GtkEntry* exp_entry = GTK_ENTRY(arguments->pdata[0]);
  GtkEntry* var_entry = GTK_ENTRY(arguments->pdata[1]);
  GtkTextView* view = GTK_TEXT_VIEW(arguments->pdata[2]);
  GtkTextBuffer* buffer = gtk_text_view_get_buffer(view);

  int err = 0;
  gchar* expression = (gchar*)gtk_entry_get_text(exp_entry);
  list* lexems = parse_line(expression, &err);
  if (err == 0) {
    list* parsed = parse_to_polish(lexems, &err);
    if (err == 0) {
      double input;
      if (safe_get_input(var_entry, lexems, &input) == 0) {
        double res = apply_polish(input, parsed, &err);
        if (err == 0) {
          char buff[100] = {0};
          sprintf(buff, "%.7f", res);
          gtk_text_buffer_set_text(buffer, buff, (int)strlen(buff));
        } else {
          error_msg_to_buffer(buffer, "Invalid expression!");
        }
      } else {
        error_msg_to_buffer(buffer, "Wrong or missing variable!");
      }
    } else {
      error_msg_to_buffer(buffer, "Polish notation error!");
    }
    list_destroy(parsed);
  } else {
    error_msg_to_buffer(buffer, "Parsing error!");
  }
  list_destroy_with_lexem(lexems);
}

gboolean on_key_press(GtkWidget* widget, GdkEventKey* event,
                      gpointer user_data) {
  switch (event->keyval) {
    case GDK_KEY_equal:
      printf("=         was pressed on %s", gtk_widget_get_name(widget));
      evaluate_expression(user_data);
      break;
    default:
      printf("%-10s was pressed on %s\n", gdk_keyval_name(event->keyval),
             gtk_widget_get_name(widget));
      break;
  }
  return FALSE;
}
