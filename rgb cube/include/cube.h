#pragma once

#include "inttypes.h"
#include "stdlib.h"

#define X_LENGTH 256
#define Y_LENGTH 256
#define SIDE_LENGTH (X_LENGTH * Y_LENGTH * 3)

#define RED(x, y, pixels, pixels_length) get_pixel(x, y, pixels, pixels_length)
#define GREEN(x, y, pixels, pixels_length) (get_pixel(x, y, pixels, pixels_length) + 1)
#define BLUE(x, y, pixels, pixels_length) (get_pixel(x, y, pixels, pixels_length) + 2)

uint8_t *get_pixel(uint8_t x, uint8_t y, uint8_t *pixels, size_t pixels_length);

uint8_t *get_blank_wall();

uint8_t *get_front_wall();
uint8_t *get_prime_front_wall();

uint8_t *get_top_wall();
uint8_t *get_prime_top_wall();

uint8_t *get_side_wall();
uint8_t *get_prime_side_wall();

void rotate_90(uint8_t **pixels);
