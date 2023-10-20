#pragma once

#include "gtk/gtk.h"
#include "color_converter/cmyk.h"
#include "colors_context.h"

GtkWidget *cmyk_container_new(ColorsContext *context);
