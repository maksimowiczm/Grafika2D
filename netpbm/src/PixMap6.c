#include "netpbm/PixMap6.h"

#include "string.h"

typedef struct {
  PixMapHeader header;
  size_t headerEnd;
} ReadHeaderResponse;

static ReadHeaderResponse
read_header(const char *input, size_t length) {
  int8_t values_read = 0;
  PixMapHeader header = {0};

  size_t i;
  for (i = 2; i < length; i++) {

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

    // read 3 values, break at 3rd
    if (values_read == 0) {
      header.width = num;
    } else if (values_read == 1) {
      header.height = num;
    } else {
      header.max_value = num;
      break;
    }

    values_read++;
  }

  while (input[i++] != '\n');

  return (ReadHeaderResponse) {header, i};
}

static PixMap6Image *
P6_read_from_string(const char *input, size_t length) {
  ReadHeaderResponse header_response = read_header(input, length);
  PixMapHeader header = header_response.header;

  size_t pixels_count = header.height * header.width * 3;
  void *pixels = malloc(sizeof(PixMapHeader) + sizeof(uint8_t) * pixels_count);
  memset(pixels, 0, sizeof(PixMapHeader) + sizeof(uint8_t) * pixels_count);

  memcpy(pixels + sizeof(PixMapHeader), input + header_response.headerEnd, pixels_count);

  PixMap6Image *map = (PixMap6Image *) pixels;
  map->header = header;

  return map;
}

PixMap6Image *netpbm_P6_read_from_file(FILE *file) {
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
  PixMap6Image *map = P6_read_from_string(buffer, bytes_read);

  free(buffer);
  return map;
}
