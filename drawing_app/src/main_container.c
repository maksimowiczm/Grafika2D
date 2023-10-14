#include "drawing_app/widgets/main_container.h"


#include "drawing_app/widgets/drawable_area.h"
#include "drawing_app/widgets/side_menu.h"


GtkWidget *new_main_container(Context *context) {
  GtkWidget *container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);

  GtkWidget *drawable_area = new_drawable_area(context);
  gtk_box_append(GTK_BOX(container), drawable_area);

  GtkWidget *side_menu = new_side_menu(context);
  gtk_box_append(GTK_BOX(container), side_menu);

  return container;
}
