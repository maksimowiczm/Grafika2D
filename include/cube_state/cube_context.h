#pragma once

#include "cube_state.h"
#include "cube_context.h"
#include "gtk/gtk.h"

typedef struct cube_context CubeContext;

struct cube_context {
  GtkPicture **picture;
  ICubeState state;
};

CubeContext *cube_context_new(GtkPicture *picture);

void cube_context_free(CubeContext *context);

void cube_context_left(CubeContext *context);
void cube_context_right(CubeContext *context);
void cube_context_up(CubeContext* context);
void cube_context_down(CubeContext* context);
