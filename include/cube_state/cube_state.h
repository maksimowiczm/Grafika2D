#pragma once

#include "inttypes.h"

enum CubeState {
  Front,
  Top,
  Side,
  FrontPrime,
  TopPrime,
  SidePrime,
};

typedef struct cube_state ICubeState;

struct cube_state {
  enum CubeState name;
  ICubeState (*up)();
  ICubeState (*right)();
  ICubeState (*down)();
  ICubeState (*left)();
  uint8_t* (*get_pixels)();
};

ICubeState cube_state_get_cube_state(enum CubeState type);

ICubeState cube_state_top_get();
ICubeState cube_state_prime_top_get();

ICubeState cube_state_front_get();
ICubeState cube_state_prime_front_get();

ICubeState cube_state_side_get();
ICubeState cube_state_prime_side_get();
