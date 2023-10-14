#include "netpbm/PixMap3.h"

#include "string.h"

static PixMap3Image *
netpbm_P3_read_from_string(const char *input, size_t length) {
  uint16_t *pixels = malloc(sizeof(uint16_t) * length);
  memset(pixels, 0, sizeof(uint16_t) * length);
  size_t pixels_pos = 0;

  for (size_t i = 2; i < length; i++) {

    // skip comments
    if (input[i] == '#') {
      while (input[i++] != '\n');
    }

    // skip not digit
    if (input[i] < '0' || '9' < input[i]) {
      continue;
    }

    // read number
    char *end;
    uint16_t num = strtol(input + i, &end, 10);
    i = end - input;
    *(pixels + pixels_pos++) = num;
  }

  PixMap3Image *map = (PixMap3Image *) pixels;

  return map;
}

PixMap3Image *netpbm_P3_read_from_file(FILE *file) {
  // file length
  fseek(file, 0L, SEEK_END);
  long length = ftell(file);
  rewind(file);

  // read to buffer
  char *buffer = malloc(length);
  if (buffer == NULL) {
    return NULL;
  }
  memset(buffer, 0, length);
  size_t bytes_read = fread(buffer, 1, length, file);
  PixMap3Image *map = netpbm_P3_read_from_string(buffer, bytes_read);

  free(buffer);
  return map;
}
