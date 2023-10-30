#include "color_ui/internal/uint8_picker.h"
#include "shared/uint8.h"
#include "color_ui/internal/colors_context.h"

static uint8_t
get_number_from_input(GtkWidget *widget, uint8_t max_value) {
  GtkText *text = (GtkText *) widget;
  const char *input = gtk_entry_buffer_get_text(gtk_text_get_buffer(text));
  char *end;
  long number = strtol(input, &end, 10);
  if (number < 0) {
    number = 0;
    gtk_entry_buffer_set_text(gtk_text_get_buffer(text), "0", 1);
  } else if (number > max_value) {
    number = max_value;
    gtk_entry_buffer_set_text(gtk_text_get_buffer(text), uint8_to_string(max_value), 3);
  }

  return number;
}

static gboolean
on_input_change(GtkWidget *widget, onChangeWidgets *widgets) {
  uint8_t value = get_number_from_input(widget, widgets->max_value);
  *widgets->value = value;
  gtk_range_set_value(GTK_RANGE(widgets->scale), value);
  colors_context_color_update(GET_CONTEXT, widgets->mode);
  return TRUE;
}

static gboolean
on_scale_change(GtkWidget *scale, onChangeWidgets *widgets) {
  uint8_t value = gtk_range_get_value(GTK_RANGE(widgets->scale));
  gtk_entry_buffer_set_text(
      gtk_text_get_buffer(GTK_TEXT(widgets->input)),
      uint8_to_string(value),
      3);
  colors_context_color_update(GET_CONTEXT, widgets->mode);
  return TRUE;
}

static void
on_input_destroy(GtkWidget *widget, gpointer user_data) {
  free(user_data);
}

GtkWidget *gtk_ui_uint8_picker(const char *label, uint8_t *value, uint8_t max_value, enum ColorMode mode, int index) {
  GtkWidget *container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  GtkWidget *text_container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
  gtk_box_append(GTK_BOX(container), text_container);

  GtkWidget *gtk_label = gtk_label_new(label);
  GtkWidget *input = gtk_text_new();
  gtk_entry_buffer_set_text(gtk_text_get_buffer(GTK_TEXT(input)), "0", 1);
  gtk_box_append(GTK_BOX(text_container), gtk_label);
  gtk_box_append(GTK_BOX(text_container), input);

  GtkWidget *scale = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0, max_value, 1);
  gtk_box_append(GTK_BOX(container), scale);

  GET_CONTEXT->inputs[index] = input;
  GET_CONTEXT->scales[index] = scale;

  onChangeWidgets *widgets = malloc(sizeof(*widgets));
  widgets->value = value;
  widgets->scale = scale;
  widgets->input = input;
  widgets->max_value = max_value;
  widgets->mode = mode;

  g_signal_connect(input, "changed", G_CALLBACK(on_input_change), widgets);
  g_signal_connect(scale, "value-changed", G_CALLBACK(on_scale_change), widgets);
  g_signal_connect(container, "destroy", G_CALLBACK(on_input_destroy), widgets);

  return container;
}
