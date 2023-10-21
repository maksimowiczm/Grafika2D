#include "cube_state/cube_state.h"
#include "cube.h"

ICubeState cube_state_get_cube_state(enum CubeState type) {
  switch (type) {
    case Front: return cube_state_front_get();
    case Top: return cube_state_top_get();
    case Side: return cube_state_side_get();
    case FrontPrime: return cube_state_prime_front_get();
    case TopPrime: return cube_state_prime_top_get();
    case SidePrime: return cube_state_prime_side_get();
  }
}

#define STATE_NEW(ENUM_NAME, NAME, UP, RIGHT, DOWN, LEFT, PIXELS) \
ICubeState NAME() {                                               \
  ICubeState state;                                               \
  state.name = ENUM_NAME;                                         \
  state.up = UP;                                                  \
  state.left = LEFT;                                              \
  state.right = RIGHT;                                            \
  state.down = DOWN;                                              \
  state.get_pixels = PIXELS;                                      \
  return state;                                                   \
}

STATE_NEW(
    Front,
    cube_state_front_get,
    cube_state_top_get,
    cube_state_side_get,
    cube_state_prime_top_get,
    cube_state_prime_side_get,
    get_front_wall
)

STATE_NEW(
    FrontPrime,
    cube_state_prime_front_get,
    cube_state_prime_top_get,
    cube_state_prime_side_get,
    cube_state_top_get,
    cube_state_side_get,
    get_prime_front_wall
)

STATE_NEW(
    Top,
    cube_state_top_get,
    cube_state_prime_front_get,
    cube_state_side_get,
    cube_state_front_get,
    cube_state_prime_side_get,
    get_top_wall
)

STATE_NEW(
    TopPrime,
    cube_state_prime_top_get,
    cube_state_front_get,
    cube_state_side_get,
    cube_state_prime_front_get,
    cube_state_prime_side_get,
    get_prime_top_wall
)

STATE_NEW(
    Side,
    cube_state_side_get,
    cube_state_top_get,
    cube_state_prime_front_get,
    cube_state_prime_top_get,
    cube_state_front_get,
    get_side_wall
)

STATE_NEW(
    SidePrime,
    cube_state_prime_side_get,
    cube_state_top_get,
    cube_state_front_get,
    cube_state_prime_top_get,
    cube_state_prime_front_get,
    get_prime_side_wall
)
