#include "cube_state/cube_context.h"

#include "stdlib.h"
#include "cube.h"

static void draw(CubeContext *context, bool init) {
  uint8_t *upper;
  uint8_t *side;
  uint8_t *front_pixels = context->state.get_pixels();
  if (context->up) {
    side = context->state.right().get_pixels();
    upper = get_top_wall();
  } else {
    upper = get_prime_top_wall();
    side = context->state.left().get_pixels();

    rotate_90(&upper);
    rotate_90(&upper);
    rotate_90(&side);
    rotate_90(&side);
    rotate_90(&front_pixels);
    rotate_90(&front_pixels);
  }

  if (!init) {
    static uint8_t rot;
    if (context->right) {
      rot++;
    } else {
      rot--;
    }
    rot = rot % 4;

    if (rot == 1) {
      rotate_90(&upper);
    } else if (rot == 2) {
      rotate_90(&upper);
      rotate_90(&upper);
    } else if (rot == 3) {
      rotate_90(&upper);
      rotate_90(&upper);
      rotate_90(&upper);
    }
  }

  const size_t BUFFER_SIZE = SIDE_LENGTH * 4;
  uint8_t *buffer = malloc(BUFFER_SIZE);
  memset(buffer, 0, BUFFER_SIZE);

#define WIDTH (X_LENGTH * 3)
#define HEIGHT (Y_LENGTH)

  for (size_t y = 0; y < HEIGHT; y++) {
    memcpy(buffer + SIDE_LENGTH * 2 + WIDTH * 2 * y, front_pixels + WIDTH * y, WIDTH);
  }

  int step = WIDTH;
  for (size_t y = 0; y < HEIGHT; y++) {
    memcpy(buffer + WIDTH * 2 * y + step, upper + WIDTH * y, WIDTH);
    step -= 3;
  }

  GdkPixbuf *pixbuf = gdk_pixbuf_new_from_data(
      buffer,
      GDK_COLORSPACE_RGB,
      FALSE,
      8,
      X_LENGTH * 2,
      Y_LENGTH * 2,
      X_LENGTH * 3 * 2,
      NULL,
      NULL
  );

  GdkPixbuf *rotated = gdk_pixbuf_rotate_simple(pixbuf, 270);
  uint8_t *rotated_buffer = gdk_pixbuf_get_pixels(rotated);

  step = 0;
  rotate_90(&side);
  for (size_t y = HEIGHT; y < HEIGHT * 2; y++) {
    memcpy(rotated_buffer + WIDTH * 2 * y + step, side + WIDTH * (y - HEIGHT), WIDTH);
    step += 3;
  }

  GdkPixbuf *out = gdk_pixbuf_rotate_simple(rotated, 90);

  gtk_picture_set_pixbuf(GTK_PICTURE(*context->picture), out);
}

CubeContext *cube_context_new(GtkPicture *picture) {
  CubeContext *context = malloc(sizeof(*context));
  context->state = cube_state_get_cube_state(Front);
  context->picture = malloc(sizeof(GtkPicture *));
  *context->picture = picture;
  context->right = true;
  context->up = true;
  draw(context, true);
  return context;
}

void cube_context_free(CubeContext *context) {
  free(context->picture);
  free(context);
}

void cube_context_left(CubeContext *context) {
  if (context->up) {
    context->state = context->state.left();
  } else {
    context->state = context->state.right();
  }
  context->right = false;
  draw(context, false);
}

void cube_context_right(CubeContext *context) {
  if (context->up) {
    context->state = context->state.right();
  } else {
    context->state = context->state.left();
  }
  context->right = true;
  draw(context, false);
}
