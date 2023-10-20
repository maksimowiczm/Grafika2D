#pragma once

#include "gtk/gtk.h"
#include "inttypes.h"

typedef struct {
  GtkWidget *input;
  GtkWidget *scale;
  uint8_t *value;
} onChangeWidgets;

GtkWidget *gtk_ui_uint8_picker(const char *label, uint8_t *value);
