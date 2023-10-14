#pragma once

#include "netpbm/PixMap.h"
#include "stdlib.h"

typedef PixMapImage PixMap3Image;

PixMap3Image *netpbm_P3_read_from_string(const char *input, size_t length);
