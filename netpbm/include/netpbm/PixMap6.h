#pragma once

#include "netpbm/PixMap.h"
#include "stdlib.h"
#include "stdio.h"

typedef PixMapImage PixMap6Image;

PixMap6Image *netpbm_P6_read_from_file(FILE *file);
