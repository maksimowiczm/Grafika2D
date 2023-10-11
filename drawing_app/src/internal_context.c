#include "drawing_app/context/internal_context.h"

#include "drawing_app/context.h"
#include "drawio/draw.h"
#include "drawio/draw_methods.h"
#include "drawing_app/state/state.h"

void internal_context_buffer_clear(Context *context) {
  context->buffer.buffer_current_size = 0;
  memset(context->buffer.buffer, 0, sizeof(Point) * context->buffer.buffer_size);
}

void internal_context_buffer_add(Context *context, Point point) {
  context->buffer.buffer[context->buffer.buffer_current_size] = point;
  context->buffer.buffer_current_size++;
}

void internal_context_shapes_clear(Context *context) {
  context_free(context, false);
  memset(context->shapes, 0, sizeof(DrawableShape *) * context->shapes_length);
}

void internal_context_initialize_user_inputs(Context *context, size_t length) {
  context->user_input.inputs_length = length;
  context->user_input.inputs = malloc(sizeof(GtkWidget *) * length);
  memset(context->user_input.inputs, 0, sizeof(GtkWidget *) * length);
}

void internal_context_free_user_inputs(Context *context) {
  free(context->user_input.inputs);
}

void internal_context_hide_user_inputs(Context *context) {
  for (int i = 0; i < context->user_input.inputs_length; i++) {
    GtkWidget *input = context->user_input.inputs[i];
    gtk_widget_set_visible(input, false);
  }
}

void internal_context_show_user_inputs(Context *context) {
  internal_context_hide_user_inputs(context);
  for (int i = 0; i < shapes_point_count_to_create(context->currentType); i++) {
    GtkWidget *input = context->user_input.inputs[i];
    gtk_widget_set_visible(input, true);
  }
}

void internal_context_load_shape_to_user_input(Context *context, DrawableShape *shape) {
  for (int i = 0; i < shapes_point_count_to_create(context->currentType); i++) {
    GtkWidget *container = context->user_input.inputs[i];
    GtkWidget *entry = gtk_widget_get_last_child(container);
    GtkEntryBuffer *buffer = gtk_entry_get_buffer(GTK_ENTRY(entry));
    Point point = shape->shape->points[i];
    char *string = shapes_point_to_string_int(point);
    gtk_entry_buffer_set_text(buffer, string, strlen(string));
    free(string);
    context_set_shape(context, shape->shape->header.type);
    internal_context_show_user_inputs(context);
  }
}

DrawableShape *internal_context_shapes_closest_to_point(Context *context, Point point) {
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

inline bool handle_draw_button_click(Context *context) {
  return (*context->state)->handle_draw_button_click(context);
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

void internal_context_shapes_add(Context *context) {
  Shape *shape = shapes_new_shape(context->currentType, context->buffer.buffer);
  DrawableShape *drawableShape = drawio_new_drawableShape(shape, drawio_get_draw_method(context->currentType));
  internal_context_add_shape(context, drawableShape, true);
  internal_context_buffer_clear(context);
}

void internal_context_state_change(Context *context, enum StateEnum newType) {
  State *newState = state_get_state(newType);

  free(*context->state);
  *context->state = newState;

  context_redraw(context);
}
