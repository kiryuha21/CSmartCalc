#include <ctype.h>

#include "../s21_smart_calc.h"

int is_empty(const char* string) {
  int result = 0;
  if (string == NULL || *string == '\0') {
    result = 1;
  } else {
    for (; *string != '\0'; ++string) {
      if (isspace(*string)) {
        result = 1;
      }
    }
  }
  return result;
}

int safe_get_double_from_str(const char* str, double* val) {
  if (strchr(str, ',') != NULL) {
    return ERR;
  }

  int return_code = 0;
  char* input_copy = calloc(strlen(str) + 1, sizeof(char));
  strcpy(input_copy, str);

  char* endptr = NULL;
  safe_solo_char_replace(input_copy, '.', ',');
  *val = strtod(input_copy, &endptr);
  if (*endptr != '\0') {
    return_code = ERR;
  }
  free(input_copy);

  return return_code;
}

int create_styled_window(GtkBuilder** builder, const char* ui_filename,
                         GtkCssProvider** provider) {
  GError* error = NULL;

  *builder = gtk_builder_new();
  if (gtk_builder_add_from_file(*builder, ui_filename, &error) == 0) {
    g_printerr("Error loading file: %s\n", error->message);
    g_clear_error(&error);
    return ERR;
  }

  *provider = gtk_css_provider_new();
  if (gtk_css_provider_load_from_path(*provider, STYLES_FILE, &error) == 0) {
    g_printerr("Error loading file: %s\n", error->message);
    g_clear_error(&error);
    return ERR;
  }
  gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                            GTK_STYLE_PROVIDER(*provider),
                                            GTK_STYLE_PROVIDER_PRIORITY_USER);
  return 0;
}

void free_array_on_close(GtkWidget* window, gpointer user_data) {
  GPtrArray* array = user_data;
  g_ptr_array_free(array, gtk_false());
  printf("%s freed array", gtk_widget_get_name(window));
}

void safe_solo_char_replace(char* str, char sym, char replacer) {
  char* search = strchr(str, sym);
  if (search != NULL) {
    size_t pos = search - str;
    str[pos] = replacer;
  }
}
