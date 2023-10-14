#pragma once

#include "netpbm/PixMap.h"
#include "stdlib.h"
#include "stdio.h"

typedef PixMapImage PixMap3Image;

PixMap3Image *netpbm_P3_read_from_file(FILE *file);
