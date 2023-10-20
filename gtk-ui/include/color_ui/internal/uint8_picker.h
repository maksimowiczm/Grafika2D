#pragma once

#include "gtk/gtk.h"
#include "inttypes.h"
#include "color_container.h"

typedef struct {
  GtkWidget *input;
  GtkWidget *scale;
  uint8_t *value;
  uint8_t max_value;
  enum ColorMode mode;
} onChangeWidgets;

GtkWidget *gtk_ui_uint8_picker(const char *label, uint8_t *value, uint8_t max_value, enum ColorMode mode, int index);
