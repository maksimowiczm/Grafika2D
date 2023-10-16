#include "netpbm/PixMap.h"

#include "ctype.h"
#include "stdlib.h"

void netpbm_convert_RGB_BGR(PixMapImage *image) {
  PixMapHeader *header = (PixMapHeader *) image;
  size_t length = header->width * header->height * 3;

  for (int i = 2; i < length; i += 3) {
    uint8_t temp = image->pixels[i];

    // r -> b
    image->pixels[i] = image->pixels[i - 2];

    // r -> b
    image->pixels[i - 2] = temp;
  }
}

void netpbm_pix_map_read_header(PixMapHeader *header, size_t *headerEnd, const char *input, size_t length) {
  int8_t values_read = 0;
  PixMapHeader fileHeader = {0};

  size_t i;
  for (i = 2; i < length; i++) {

    // skip comments
    if (input[i] == '#') {
      while (input[i] != '\n' && input[i++] != '\r');
    }

    // continue if whitespace
    if (isspace(input[i])) {
      continue;
    }

    // throw if not digit
    if (!isdigit(input[i])) {
      header = NULL;
      return;
    }

    // read number
    char *end;
    uint16_t num = strtol(input + i, &end, 10);
    i = end - input;

    // read 3 values, break at 3rd
    if (values_read == 0) {
      fileHeader.width = num;
    } else if (values_read == 1) {
      fileHeader.height = num;
    } else {
      fileHeader.max_value = num;
      break;
    }

    values_read++;
  }

  *headerEnd = i;
  *header = fileHeader;
}
