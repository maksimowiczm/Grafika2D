#pragma once

#include "windows.h"
#include "context.hpp"
extern "C" {
#include "netpbm/PixMap.h"
}

void draw_pix_map_image(HWND hwnd, Image *image);
