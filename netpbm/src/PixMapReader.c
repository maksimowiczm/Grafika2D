#include "netpbm/PixMapReader.h"

#include "netpbm/PixMap3.h"
#include "netpbm/PixMap6.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

static PixMap3Image *
netpbm_read_buffer_from_file(FILE *file, PixMapImage *(*reader)(const char *input, size_t length)) {
  // file length
  _fseeki64(file, 0L, SEEK_END);
  int64_t length = _ftelli64(file);
  rewind(file);

  // read to buffer
  char *buffer = malloc(length);
  if (buffer == NULL) {
    return NULL;
  }
  memset(buffer, 0, length);
  size_t bytes_read = fread(buffer, 1, length, file);
  PixMap3Image *map = reader(buffer, bytes_read);

  free(buffer);
  return map;
}

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
    image = netpbm_read_buffer_from_file(file, netpbm_P3_read_from_string);
  } else if (strcmp(buffer, "P6") == 0) {
    image = netpbm_read_buffer_from_file(file, netpbm_P6_read_from_string);
  }

  fclose(file);
  return image;
}
