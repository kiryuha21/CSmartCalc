#include <math.h>

#include "../s21_smart_calc.h"

void annuity_calculation(CreditComponents *components) {
  double monthly_rate = components->rate / 1200;
  double monthly_payment = components->amount * monthly_rate /
                           (1 - pow(1 + monthly_rate, -components->term));
  double total_payment = monthly_payment * components->term;
  double overpayment = total_payment - components->amount;

  gchar *s_monthly_payment = calloc(100, sizeof(char));
  gchar *s_total_payment = calloc(100, sizeof(char));
  gchar *s_overpayment = calloc(100, sizeof(char));

  sprintf(s_monthly_payment, "%.7f", monthly_payment);
  sprintf(s_total_payment, "%.7f", total_payment);
  sprintf(s_overpayment, "%.7f", overpayment);

  GtkTextBuffer *monthly_payment_buffer =
      gtk_text_view_get_buffer(components->monthly_payment);
  gtk_text_buffer_set_text(monthly_payment_buffer, s_monthly_payment,
                           (int)strlen(s_monthly_payment));

  GtkTextBuffer *overpayment_buffer =
      gtk_text_view_get_buffer(components->overpayment);
  gtk_text_buffer_set_text(overpayment_buffer, s_overpayment,
                           (int)strlen(s_overpayment));

  GtkTextBuffer *total_payment_buffer =
      gtk_text_view_get_buffer(components->total_payment);
  gtk_text_buffer_set_text(total_payment_buffer, s_total_payment,
                           (int)strlen(s_total_payment));

  free(s_monthly_payment);
  free(s_total_payment);
  free(s_overpayment);
}

void differentiated_calculation(CreditComponents *components) {
  double monthly_rate = components->rate / 1200;
  double base_monthly_payment = components->amount / components->term;
  double total_payment = 0;
  for (int i = 0; i < components->term; ++i) {
    total_payment += base_monthly_payment +
        (components->amount - base_monthly_payment * i) * monthly_rate;
  }
  double overpayment = total_payment - components->amount;

  gchar *s_monthly_payment = calloc(100, sizeof(char));
  gchar *s_total_payment = calloc(100, sizeof(char));
  gchar *s_overpayment = calloc(100, sizeof(char));

  sprintf(s_monthly_payment, "%.7f + (%.7f - %.7f * (N - 1)) * %.7f",
          base_monthly_payment, components->amount, base_monthly_payment,
          monthly_rate);
  sprintf(s_total_payment, "%.7f", total_payment);
  sprintf(s_overpayment, "%.7f", overpayment);

  GtkTextBuffer *monthly_payment_buffer =
      gtk_text_view_get_buffer(components->monthly_payment);
  gtk_text_buffer_set_text(monthly_payment_buffer, s_monthly_payment,
                           (int)strlen(s_monthly_payment));

  GtkTextBuffer *overpayment_buffer =
      gtk_text_view_get_buffer(components->overpayment);
  gtk_text_buffer_set_text(overpayment_buffer, s_overpayment,
                           (int)strlen(s_overpayment));

  GtkTextBuffer *total_payment_buffer =
      gtk_text_view_get_buffer(components->total_payment);
  gtk_text_buffer_set_text(total_payment_buffer, s_total_payment,
                           (int)strlen(s_total_payment));

  free(s_monthly_payment);
  free(s_total_payment);
  free(s_overpayment);
}

void credit_calculation(GtkWidget *widget, gpointer data) {
  UNUSED(widget);

  GPtrArray *arguments = data;
  GtkRadioButton *annuity = arguments->pdata[0];
  const gchar *s_amount = gtk_entry_get_text(arguments->pdata[1]);
  const gchar *s_term = gtk_entry_get_text(arguments->pdata[2]);
  const gchar *s_rate = gtk_entry_get_text(arguments->pdata[3]);
  GtkTextView *monthly_payment = arguments->pdata[4];
  GtkTextView *overpayment = arguments->pdata[5];
  GtkTextView *total_payment = arguments->pdata[6];
  GtkLabel *label = arguments->pdata[7];

  double amount = 0, term = 0, rate = 0;
  if (safe_get_double_from_str(s_amount, &amount) == ERR ||
      safe_get_double_from_str(s_term, &term) == ERR ||
      safe_get_double_from_str(s_rate, &rate) == ERR) {
    gtk_label_set_label(label, INVALID_NUMBER_MSG);
    return;
  }

  if (amount <= 1e-7 || term <= 1e-7 || rate <= 1e-7) {
    gtk_label_set_label(label, INVALID_EXPRESSION_MSG);
    return;
  }

  CreditComponents components = {amount,          term,        rate,
                                 monthly_payment, overpayment, total_payment};

  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(annuity)) == gtk_true()) {
    annuity_calculation(&components);
  } else {
    differentiated_calculation(&components);
  }
}

void create_credit_calculator(GtkWidget *widget, gpointer data) {
  UNUSED(widget);
  UNUSED(data);

  GtkBuilder *builder = NULL;
  GtkCssProvider *provider = NULL;
  if (create_styled_window(&builder, CREDIT_UI_FILE, &provider) == ERR) {
    return;
  }

  GtkRadioButton *annuity_radio =
      GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "annuity_radio"));
  GtkEntry *amount = GTK_ENTRY(gtk_builder_get_object(builder, "amount_entry"));
  GtkEntry *term = GTK_ENTRY(gtk_builder_get_object(builder, "term_entry"));
  GtkEntry *rate = GTK_ENTRY(gtk_builder_get_object(builder, "rate_entry"));
  GtkLabel *label = GTK_LABEL(gtk_builder_get_object(builder, "credit_label"));
  GtkTextView *monthly_payment =
      GTK_TEXT_VIEW(gtk_builder_get_object(builder, "monthly_payment_view"));
  GtkTextView *overpayment =
      GTK_TEXT_VIEW(gtk_builder_get_object(builder, "overpayment_view"));
  GtkTextView *total_payment =
      GTK_TEXT_VIEW(gtk_builder_get_object(builder, "total_payment_view"));

  GPtrArray *arguments_array = g_ptr_array_new();
  g_ptr_array_add(arguments_array, annuity_radio);
  g_ptr_array_add(arguments_array, amount);
  g_ptr_array_add(arguments_array, term);
  g_ptr_array_add(arguments_array, rate);
  g_ptr_array_add(arguments_array, monthly_payment);
  g_ptr_array_add(arguments_array, overpayment);
  g_ptr_array_add(arguments_array, total_payment);
  g_ptr_array_add(arguments_array, label);

  GObject *graph_window = gtk_builder_get_object(builder, "credit_window");
  g_signal_connect(graph_window, "destroy", G_CALLBACK(gtk_window_close), NULL);
  g_signal_connect(graph_window, "destroy", G_CALLBACK(free_array_on_close),
                   arguments_array);

  GObject *calculate_button =
      gtk_builder_get_object(builder, "calculate_button");
  g_signal_connect(calculate_button, "clicked", G_CALLBACK(credit_calculation),
                   arguments_array);
}