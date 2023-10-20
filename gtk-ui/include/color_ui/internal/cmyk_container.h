#pragma once

#include "gtk/gtk.h"
#include "color_converter/cmyk.h"

GtkWidget *cmyk_container_new(ColorCmyk *color);
