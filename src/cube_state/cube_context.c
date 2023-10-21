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

  int step = X_LENGTH * 3;
  for (size_t y = 0; y < Y_LENGTH; y++) {
    memcpy(buffer + Y_LENGTH * 3 * 2 * y + step, upper + Y_LENGTH * 3 * y, Y_LENGTH * 3);
    memcpy(buffer + SIDE_LENGTH * 2 + Y_LENGTH * 3 * 2 * y, front_pixels + Y_LENGTH * 3 * y, Y_LENGTH * 3);
    memcpy(buffer + SIDE_LENGTH * 2 + Y_LENGTH * 3 + Y_LENGTH * 3 * 2 * y, side + Y_LENGTH * 3 * y, Y_LENGTH * 3);
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

  gtk_picture_set_pixbuf(GTK_PICTURE(*context->picture), pixbuf);
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
