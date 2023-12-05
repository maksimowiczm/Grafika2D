#include "cube.h"

#include "string.h"

uint8_t *get_pixel(uint8_t x, uint8_t y, uint8_t *pixels, size_t pixels_length) {
  size_t pos = X_LENGTH * y * 3 + x * 3;
  if (pos > pixels_length) {
    return NULL;
  }

  return &pixels[pos];
}

uint8_t *get_blank_wall() {
  uint8_t *wall = malloc(SIDE_LENGTH);
  memset(wall, 255, SIDE_LENGTH);
  return wall;
}

uint8_t *get_front_wall() {
  uint8_t *wall = malloc(SIDE_LENGTH);

  for (int x = 0; x < X_LENGTH; x++) {
    for (int y = 0; y < Y_LENGTH; y++) {
      *RED(x, y, wall, SIDE_LENGTH) = 255;
      *GREEN(x, y, wall, SIDE_LENGTH) = 255 - y;
      *BLUE(x, y, wall, SIDE_LENGTH) = x;
    }
  }

  return wall;
}

uint8_t *get_prime_front_wall() {
  uint8_t *wall = malloc(SIDE_LENGTH);

  for (int x = 0; x < X_LENGTH; x++) {
    for (int y = 0; y < Y_LENGTH; y++) {
      *RED(x, y, wall, SIDE_LENGTH) = 0;
      *GREEN(x, y, wall, SIDE_LENGTH) = 255 - y;
      *BLUE(x, y, wall, SIDE_LENGTH) = 255 - x;
    }
  }

  return wall;
}

uint8_t *get_top_wall() {
  uint8_t *wall = malloc(SIDE_LENGTH);

  for (int x = 0; x < X_LENGTH; x++) {
    for (int y = 0; y < Y_LENGTH; y++) {
      *RED(x, y, wall, SIDE_LENGTH) = y;
      *GREEN(x, y, wall, SIDE_LENGTH) = 255;
      *BLUE(x, y, wall, SIDE_LENGTH) = x;
    }
  }

  return wall;
}

uint8_t *get_prime_top_wall() {
  uint8_t *wall = malloc(SIDE_LENGTH);

  for (int x = 0; x < X_LENGTH; x++) {
    for (int y = 0; y < Y_LENGTH; y++) {
      *RED(x, y, wall, SIDE_LENGTH) = 255 - y;
      *GREEN(x, y, wall, SIDE_LENGTH) = 0;
      *BLUE(x, y, wall, SIDE_LENGTH) = x;
    }
  }

  return wall;
}

uint8_t *get_side_wall() {
  uint8_t *wall = malloc(SIDE_LENGTH);

  for (int x = 0; x < X_LENGTH; x++) {
    for (int y = 0; y < Y_LENGTH; y++) {
      *RED(x, y, wall, SIDE_LENGTH) = 255 - x;
      *GREEN(x, y, wall, SIDE_LENGTH) = 255 - y;
      *BLUE(x, y, wall, SIDE_LENGTH) = 255;
    }
  }

  return wall;
}

uint8_t *get_prime_side_wall() {
  uint8_t *wall = malloc(SIDE_LENGTH);

  for (int x = 0; x < X_LENGTH; x++) {
    for (int y = 0; y < Y_LENGTH; y++) {
      *RED(x, y, wall, SIDE_LENGTH) = x;
      *GREEN(x, y, wall, SIDE_LENGTH) = 255 - y;
      *BLUE(x, y, wall, SIDE_LENGTH) = 0;
    }
  }

  return wall;
}

void rotate_90(uint8_t **pixels) {
  uint8_t rotated[SIDE_LENGTH];
  memset(rotated, 255, SIDE_LENGTH);

  int xx = X_LENGTH - 1, yy = -1;
  for (int x = 0; x < X_LENGTH; x++) {
    xx = X_LENGTH - 1;
    yy++;
    for (int y = 0; y < Y_LENGTH; y++) {
      *RED(xx, yy, rotated, SIDE_LENGTH) = *RED(x, y, *pixels, SIDE_LENGTH);
      *GREEN(xx, yy, rotated, SIDE_LENGTH) = *GREEN(x, y, *pixels, SIDE_LENGTH);
      *BLUE(xx, yy, rotated, SIDE_LENGTH) = *BLUE(x, y, *pixels, SIDE_LENGTH);
      xx--;
    }
  }

  memcpy(*pixels, rotated, SIDE_LENGTH);
}
