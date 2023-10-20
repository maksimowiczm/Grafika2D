#include "shared/uint8.h"

#include "stdlib.h"
#include "stdio.h"
#include "string.h"

char * uint8_to_string(uint8_t number) {
  char *str = malloc(4);
  memset(str, 0, 4);
  sprintf(str, "%d", number);
  return str;
}
