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
    char* endptr;
    *input = strtod(str_value, &endptr);
    if (*endptr != '\0') {
      return_code = ERR;
    }
  }
  return return_code;
}

void error_msg_to_buffer(GtkTextBuffer* buffer, char* text) {
  gtk_text_buffer_set_text(buffer, text, (int)strlen(text));
}

void evaluate_expression(EvaluationComponents* components) {
  GtkEntry* exp_entry = GTK_ENTRY(components->expression_input);
  GtkEntry* var_entry = GTK_ENTRY(components->variable_input);
  GtkTextView* view = GTK_TEXT_VIEW(components->result_view);
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
        char buff[10] = {0};
        sprintf(buff, "%f", res);
        gtk_text_buffer_set_text(buffer, buff, (int)strlen(buff));
        list_destroy(parsed);
      } else {
        error_msg_to_buffer(buffer, "Wrong or missing variable!");
      }
    } else {
      error_msg_to_buffer(buffer, "Polish notation error!");
    }
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
