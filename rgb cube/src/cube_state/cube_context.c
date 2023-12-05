#include "cube_state/cube_context.h"

#include "stdlib.h"
#include "cube.h"

static void draw(CubeContext *context, bool init, bool flip) {
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
    if (!flip) {
      if (context->up) {
        if (context->right) {
          rot++;
        } else {
          rot--;
        }
      } else {
        if (context->right) {
          rot--;
        } else {
          rot++;
        }
      }

      rot = rot % 4;
    }

    if (context->up) {
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
    } else {
      if (rot == 3) {
        rotate_90(&upper);
      } else if (rot == 2) {
        rotate_90(&upper);
        rotate_90(&upper);
      } else if (rot == 1) {
        rotate_90(&upper);
        rotate_90(&upper);
        rotate_90(&upper);
      }
    }

  }

#define WIDTH (X_LENGTH * 3)
#define HEIGHT (Y_LENGTH)

  const size_t BUFFER_SIZE = SIDE_LENGTH * 4 + WIDTH * 2;
  uint8_t *buffer = malloc(BUFFER_SIZE);
  memset(buffer, 255, BUFFER_SIZE);

  for (size_t y = 0; y < HEIGHT; y++) {
    memcpy(buffer + SIDE_LENGTH * 2 + WIDTH * 2 * y, front_pixels + WIDTH * y, WIDTH);
  }

  int step = WIDTH / 2;
  int yy = 0;
  for (size_t y = 0; y < HEIGHT / 2 - 2; y += 2) {
    memcpy(buffer + SIDE_LENGTH + SIDE_LENGTH / 2 + WIDTH * 2 * yy++ + step, upper + WIDTH * 2 * y, WIDTH - 3);
    step -= 6;
  }
  memset(buffer + SIDE_LENGTH + SIDE_LENGTH / 2 + WIDTH * 2 * yy + step, 0, WIDTH - 3);

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
  yy = 1;
  memset(rotated_buffer + SIDE_LENGTH * 2 + WIDTH * 2 * 0 + step, 0, WIDTH - 3);
  for (size_t y = 0; y < HEIGHT; y += 2) {
    memcpy(rotated_buffer + SIDE_LENGTH * 2 + WIDTH * 2 * yy++ + step, side + WIDTH * y, WIDTH - 3);
    memset(rotated_buffer + SIDE_LENGTH * 2 + WIDTH * 2 * yy + step + WIDTH - 3, 0, 6);
    if ((y + 0) % 4 == 0) {
      step += 3;
    }
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
  draw(context, true, false);
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
  draw(context, false, false);
}

void cube_context_right(CubeContext *context) {
  if (context->up) {
    context->state = context->state.right();
  } else {
    context->state = context->state.left();
  }
  context->right = true;
  draw(context, false, false);
}

void cube_context_flip(CubeContext *context) {
  context->up = !context->up;
  draw(context, false, true);
}
