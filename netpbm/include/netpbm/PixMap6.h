#pragma once

#include "netpbm/PixMap.h"

typedef PixMapImage PixMap6Image;

PixMap6Image *netpbm_P6_read_from_string(const char *input, size_t length);
