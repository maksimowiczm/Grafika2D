#pragma once

#include "windows.h"
extern "C" {
#include "netpbm/PixMap.h"
}

void draw_pix_map_image(HWND hwnd, PixMapImage *image);
