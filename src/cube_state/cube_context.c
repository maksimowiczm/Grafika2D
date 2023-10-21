#include "cube_state/cube_context.h"

#include "stdlib.h"
#include "cube.h"

CubeContext *cube_context_new(GtkPicture *picture) {
  CubeContext *context = malloc(sizeof(*context));
  context->state = cube_state_get_cube_state(Front);
  context->picture = malloc(sizeof(GtkPicture *));
  *context->picture = picture;
  return context;
}

void cube_context_free(CubeContext *context) {
  free(context->picture);
  free(context);
}

static void draw(CubeContext *context) {
  GdkPixbuf *pixbuf = gdk_pixbuf_new_from_data(
      context->state.get_pixels(),
      GDK_COLORSPACE_RGB,
      FALSE,
      8,
      X_LENGTH,
      Y_LENGTH,
      X_LENGTH * 3,
      NULL,
      NULL
  );
  gtk_picture_set_pixbuf(GTK_PICTURE(*context->picture), pixbuf);
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
