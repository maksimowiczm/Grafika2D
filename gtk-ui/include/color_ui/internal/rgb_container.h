#pragma once

#include "gtk/gtk.h"
#include "color_converter/rgb.h"

GtkWidget *rgb_container_new(ColorRgb *color);
