#include "drawing_app/widgets/main_container.h"


#include "drawing_app/widgets/drawable_area.h"


GtkWidget *new_main_container(WindowState* state) {
  GtkWidget *container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);

  GtkWidget *drawable_area = new_drawable_area(state);
  gtk_box_append(GTK_BOX(container), drawable_area);

  return container;
}
