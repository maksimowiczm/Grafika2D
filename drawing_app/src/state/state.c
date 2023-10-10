#include "drawing_app/state/state.h"
#include "drawing_app/state/no_action_state.h"
#include "drawing_app/state/drawing_state.h"


State *state_get_state(enum StateEnum type) {
  if (type == NoAction) {
    return no_action_state_get();
  } else if (type == Drawing) {
    return drawing_state_get();
  }
}
