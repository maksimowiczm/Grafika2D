#include "netpbm/PixMapReader.h"

#include "stdio.h"
#include "string.h"
#include "netpbm/PixMap3.h"

PixMapImage *netpbm_PixMap_read_from_file(const char *file_path) {
  FILE *file;
  file = fopen(file_path, "r");

  if (file == NULL) {
    return NULL;
  }

  // read PPM header
  char buffer[3];
  memset(buffer, 0, 3);
  size_t bytes_read = fread(buffer, 1, 2, file);
  if (bytes_read < 2) {
    return NULL;
  }

  // P3 - ascii
  // P6 - binary
  PixMapImage *image = NULL;
  if (strcmp(buffer, "P3") == 0) {
    image = netpbm_P3_read_from_file(file);
  }

  fclose(file);
  return image;
}
