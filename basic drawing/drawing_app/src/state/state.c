#include "drawing_app/state/state.h"

#include "drawing_app/state/no_action_state.h"
#include "drawing_app/state/drawing_state.h"
#include "drawing_app/state/moving_point_state.h"
#include "drawing_app/state/moving_shape_mouse_state.h"
#include "drawing_app/state/moving_shape_state.h"

inline State *
state_get_state(enum StateEnum type) {
  switch (type) {
    case NoAction:return no_action_state_get();
    case Drawing:return drawing_state_get();
    case MovingPoint:return moving_point_state_get();
    case MovingShapeMouse:return moving_shape_mouse_state_get();
    case MovingShape:return moving_shape_state_get();
    default:return NULL;
  }
}
