#include "drawing_app/context/internal_context.h"
#include "drawing_app/context.h"


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
