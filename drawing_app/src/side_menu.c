#include "drawing_app/widgets/side_menu.h"
#include "drawing_app/widgets/user_input_container.h"
#include "drawio/draw_methods.h"

#define SHAPE_CLICKED(shape, name) \
static gboolean \
name(GtkWidget *gesture, gpointer user_data) { \
  Context* context = user_data; \
  context_set_shape(context, shape); \
  return TRUE; \
}

SHAPE_CLICKED(Line, line_click)

SHAPE_CLICKED(Quadrilateral, quadrilateral_click)

SHAPE_CLICKED(Circle, circle_click)

SHAPE_CLICKED(Rectangle, rectangle_click)

static GtkWidget *
shape_button_new(const char *label,
                 gboolean(*callback)(GtkWidget *, gpointer),
                 gpointer data) {
  GtkWidget *button = gtk_button_new_with_label(label);
  g_signal_connect(button, "clicked", G_CALLBACK(callback), data);

  return button;
}

static gboolean
button_clear_click(GtkWidget *button, gpointer user_data) {
  Context *context = user_data;
  context_clear_all(context);
  context_redraw(context);
  return TRUE;
}

static gboolean
dump_context(GtkWidget *gesture, gpointer user_data) {
  Context *context = user_data;
  FILE *file = fopen("shapes.txt", "wb");
  if (file != NULL) {
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
  }
  return TRUE;
}

static gboolean
load_context(GtkWidget *button, gpointer user_data) {
  Context *context = user_data;
  FILE *file = fopen("shapes.txt", "rb");
  if (file != NULL) {
    size_t length;
    fread(&length, sizeof(length), 1, file);
    DrawableShape *drawables = malloc(sizeof(DrawableShape) * length);

    int i;
    for (i = 0; i < length; i++) {
      if (feof(file)) {
        memset(drawables + i - 1, 0, sizeof(DrawableShape) * (length - i + 1));
        break;
      }

      DrawableShape *drawable = (drawables + i);
      memset(drawable, 0, sizeof(*drawable));

      drawable->shape = malloc(sizeof(Shape));
      memset(drawable->shape, 0, sizeof(*drawable->shape));

      fread(&drawable->shape->header.type, sizeof(drawable->shape->header.type), 1, file);
      fread(&drawable->shape->points_length, sizeof(drawable->shape->points_length), 1, file);

      drawable->shape->points = malloc(sizeof(Point) * drawable->shape->points_length);
      fread(drawable->shape->points, sizeof(Point), drawable->shape->points_length, file);

      drawable->header.draw_method = drawio_get_draw_method(drawable->shape->header.type);
      drawable->shape->header.distance = shapes_get_distance_method(drawable->shape->header.type);
    }

    fclose(file);
    context = context_replace_shapes(context, drawables, length);
    context_redraw(context);
  }

  return TRUE;
}

GtkWidget *new_side_menu(Context *context) {
  GtkWidget *container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

  GtkWidget *line = shape_button_new("line", line_click, context);
  gtk_box_append(GTK_BOX(container), line);

  GtkWidget *rectangle = shape_button_new("rectangle", rectangle_click, context);
  gtk_box_append(GTK_BOX(container), rectangle);

  GtkWidget *quadrilateral = shape_button_new("quadrilateral", quadrilateral_click, context);
  gtk_box_append(GTK_BOX(container), quadrilateral);

  GtkWidget *circle = shape_button_new("circle", circle_click, context);
  gtk_box_append(GTK_BOX(container), circle);

  GtkWidget *clear_button = gtk_button_new_with_label("clear");
  g_signal_connect(clear_button, "clicked", G_CALLBACK(button_clear_click), context);
  gtk_box_append(GTK_BOX(container), clear_button);

  GtkWidget *user_input = new_user_input_container(context);
  gtk_box_append(GTK_BOX(container), user_input);

  GtkWidget *dump_context_button = shape_button_new("save", dump_context, context);
  gtk_box_append(GTK_BOX(container), dump_context_button);

  GtkWidget *load_context_button = shape_button_new("load", load_context, context);
  gtk_box_append(GTK_BOX(container), load_context_button);

  return container;
}
