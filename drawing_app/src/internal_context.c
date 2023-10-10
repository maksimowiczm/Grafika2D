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
