#include "drawing_app/context.h"

#include "drawio/draw.h"
#include "drawing_app/context/context.h"
#include "drawing_app/context/internal_context.h"
#include "drawing_app/state/state.h"

inline bool context_handle_left_click(Context *context, Point mouse) {
  return (*context->state)->handle_left_click(context, mouse);
}

inline bool context_handle_right_click(Context *context, Point mouse) {
  return (*context->state)->handle_right_click(context, mouse);
}

inline bool context_handle_right_click_long(Context *context, Point mouse) {
  return (*context->state)->handle_right_click_long(context, mouse);
}

inline void context_handle_mouse_movement(Context *context, Point mouse) {
  return (*context->state)->handle_mouse_movement(context, mouse);
}

Context *context_new(size_t shapes_max_count, size_t buffer_max_size) {
  if (buffer_max_size < 2 || shapes_max_count < 1) {
    return NULL;
  }

  Context *context = malloc(sizeof(*context));

  context->state = malloc(sizeof(State *));
  (*context->state) = state_get_state(NoAction);

  context->buffer.buffer_size = buffer_max_size;
  context->buffer.buffer_current_size = 0;
  context->buffer.buffer = malloc(sizeof(Point) * buffer_max_size);

  context->currentType = Line;
  context->shapes_length = shapes_max_count;
  context->shapes = malloc(sizeof(DrawableShape *) * context->shapes_length);
  memset(context->shapes, 0, sizeof(DrawableShape *) * context->shapes_length);

  context->moving_shape = malloc(sizeof(DrawableShape *)); // idk ** or * ? i mean its the same but

  internal_context_initialize_user_inputs(context, buffer_max_size);

  return context;
}

Context *context_replace_shapes(Context *context, DrawableShape *shapes, size_t shapes_max_length) {
  context_clear_all(context);
  context_free(context, false);

  context->shapes = malloc(sizeof(DrawableShape *) * shapes_max_length);
  memset(context->shapes, 0, sizeof(DrawableShape *) * shapes_max_length);
  for (int i = 0; i < shapes_max_length; i++) {
    if ((shapes + i)->shape != NULL) {
      *(context->shapes + i) = shapes + i;
      (*context->shapes + i)->header.shouldBeDrawn = true;
    } else {
      *(context->shapes + i) = NULL;
    }
  }
  context->shapes_length = shapes_max_length;
}

void context_free(Context *context, bool free_self) {
  for (int i = 0; i < context->shapes_length; i++) {
    DrawableShape *shape = context->shapes[i];
    if (shape == NULL) {
      continue;
    }

    // free inner shape
    shapes_shape_free(shape->shape, true);

    // free outer shape
    drawio_drawableShape_free(shape, true);
  }

  if (!free_self) {
    return;
  }

  internal_context_free_user_inputs(context);
  if (*context->state != NULL) {
    free(*context->state);
  }
  free(context->moving_shape);
  free(context->buffer.buffer);
  free(context->state);
  free(context->shapes);
  free(context);
}

inline void context_draw(Context *context, cairo_t *cr) {
  return (*context->state)->draw(context, cr);
}

inline void context_redraw(Context *context) {
  return gtk_widget_queue_draw(context->drawing_area);
}

void context_set_shape(Context *context, enum ShapeType type) {
  context->currentType = type;
  internal_context_buffer_clear(context);
  internal_context_show_user_inputs(context);
  context_redraw(context);
}

void context_clear_all(Context *context) {
  internal_context_shapes_clear(context);
  internal_context_buffer_clear(context);
}
