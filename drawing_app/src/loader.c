#include "drawing_app/loader.h"

#include "drawio/draw_methods.h"
#include "drawing_app/context/internal_context.h"

bool loader_save_to_file(Context *context, const char *file_path) {
  FILE *file = fopen(file_path, "wb");
  if (file == NULL) {
    return false;
  }

  fwrite(&context->shapes_length, sizeof(context->shapes_length), 1, file);

  for (int i = 0; i < context->shapes_length; i++) {
    if (context->shapes[i] == NULL) {
      continue;
    }
    Shape *shape = (*(context->shapes + i))->shape;

    fwrite(&shape->header.type, sizeof(shape->header.type), 1, file);
    fwrite(&shape->points_length, sizeof(shape->points_length), 1, file);
    fwrite(shape->points, sizeof(Point), shape->points_length, file);
  }
  fclose(file);

  return true;
}

bool loader_load_from_file(Context *context, const char *file_path) {
  FILE *file = fopen(file_path, "rb");

  if (file == NULL) {
    return false;
  }

  size_t length;
  fread(&length, sizeof(length), 1, file);
  DrawableShape **drawables = malloc(sizeof(DrawableShape *) * length);
  memset(drawables, 0, sizeof(DrawableShape *) * length);

  int i;
  for (i = 0; i < length; i++) {
    if (feof(file)) {
      free((*(drawables + i - 1))->shape);
      memset(drawables + i - 1, 0, sizeof(DrawableShape *) * (length - i + 1));
      break;
    }

    DrawableShape *drawable = malloc(sizeof(*drawable));
    memset(drawable, 0, sizeof(*drawable));

    drawable->shape = malloc(sizeof(Shape));
    memset(drawable->shape, 0, sizeof(*drawable->shape));

    fread(&drawable->shape->header.type, sizeof(drawable->shape->header.type), 1, file);
    fread(&drawable->shape->points_length, sizeof(drawable->shape->points_length), 1, file);

    drawable->shape->points = malloc(sizeof(Point) * drawable->shape->points_length);
    fread(drawable->shape->points, sizeof(Point), drawable->shape->points_length, file);

    drawable->header.draw_method = drawio_get_draw_method(drawable->shape->header.type);
    drawable->shape->header.distance = shapes_get_distance_method(drawable->shape->header.type);
    drawable->header.shouldBeDrawn = true;

    *(drawables + i) = drawable;
  }

  fclose(file);

  internal_context_replace_shapes(context, drawables, length);
  return true;
}