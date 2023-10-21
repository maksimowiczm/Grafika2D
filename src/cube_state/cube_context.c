#include "cube_state/cube_context.h"

#include "stdlib.h"
#include "cube.h"

static void draw(CubeContext *context) {
  uint8_t *upper = context->state.up().get_pixels();
  uint8_t *side = context->state.right().get_pixels();
  uint8_t *front_pixels = context->state.get_pixels();

  const size_t BUFFER_SIZE = SIDE_LENGTH * 4;
  uint8_t *buffer = malloc(BUFFER_SIZE);
  memset(buffer, 0, BUFFER_SIZE);

  for (size_t y = 0; y < Y_LENGTH; y++) {
    memcpy(buffer + Y_LENGTH * 3 * 2 * y, upper + Y_LENGTH * 3 * y, Y_LENGTH * 3);
    memcpy(buffer + SIDE_LENGTH * 2 + Y_LENGTH * 3 * 2 * y, front_pixels + Y_LENGTH * 3 * y, Y_LENGTH * 3);
    memcpy(buffer + SIDE_LENGTH * 2 + Y_LENGTH * 3 + Y_LENGTH * 3 * 2 * y, side + Y_LENGTH * 3 * y, Y_LENGTH * 3);
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
  draw(context);
  return context;
}

void cube_context_free(CubeContext *context) {
  free(context->picture);
  free(context);
}

void cube_context_left(CubeContext *context) {
  context->state = context->state.left();
  draw(context);
}

void cube_context_right(CubeContext *context) {
  context->state = context->state.right();
  draw(context);
}

void cube_context_up(CubeContext *context) {
  context->state = context->state.up();
  draw(context);
}

void cube_context_down(CubeContext *context) {
  context->state = context->state.down();
  draw(context);
}
