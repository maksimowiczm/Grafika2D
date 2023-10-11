#include "drawing_app/context.h"

#include "drawio/draw.h"
#include "drawio/draw_methods.h"
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

void context_state_change(Context *context, enum StateEnum newType) {
  State *newState = state_get_state(newType);

  free(*context->state);
  *context->state = newState;

  context_redraw(context);
}

inline void context_draw(Context *context, cairo_t *cr) {
  (*context->state)->draw(context, cr);
}

inline void context_redraw(Context *context) {
  gtk_widget_queue_draw(context->drawing_area);
}

void context_set_shape(Context *context, enum ShapeType type) {
  context->currentType = type;
  internal_context_buffer_clear(context);
  context_redraw(context);
}

static ssize_t
internal_context_add_shape(Context *context, DrawableShape *shape, bool override) {
  for (int i = 0; i < context->shapes_length; i++) {
    if (context->shapes[i] == NULL) {
      *(context->shapes + i) = shape;
      return i;
    }
  }

  static size_t override_index = 0;
  if (override) {
    // free memory
    DrawableShape *drawableShape = *(context->shapes + override_index);
    shapes_shape_free(drawableShape->shape, true);
    drawio_drawableShape_free(*(context->shapes + override_index), true);

    // override
    *(context->shapes + override_index) = shape;
    override_index = (override_index + 1) % context->shapes_length;
    return override_index - 1;
  }

  return -1;
}

void context_shapes_add(Context *context) {
  Shape *shape = shapes_new_shape(context->currentType, context->buffer.buffer);
  DrawableShape *drawableShape = drawio_new_drawableShape(shape, drawio_get_draw_method(context->currentType));
  internal_context_add_shape(context, drawableShape, true);
  internal_context_buffer_clear(context);
}

void context_clear_all(Context *context) {
  internal_context_shapes_clear(context);
  internal_context_buffer_clear(context);
}

DrawableShape *context_shapes_closest_to_point(Context *context, Point point) {
  double closest = DBL_MAX;
  int index = -1;
  for (int i = 0; i < context->shapes_length; i++) {
    DrawableShape *drawable = *(context->shapes + i);
    if (drawable == NULL || !drawable->header.isDrawn) {
      continue;
    }
    Shape *shape = drawable->shape;

    double distance = shapes_shape_distance(*shape, point);
    if (distance < closest) {
      closest = distance;
      index = i;
    }
  }

  if (index != -1 && closest < 10) {
    return context->shapes[index];
  }

  return NULL;
}
