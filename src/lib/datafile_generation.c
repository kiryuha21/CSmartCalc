#include <math.h>

#include "../s21_smart_calc.h"

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
