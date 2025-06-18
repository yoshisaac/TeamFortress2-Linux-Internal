#include "config.hpp"

bool (*draw_view_model_original)(void*);

bool draw_view_model_hook(void* me) {
  if (config.visuals.hide_scope == true)
    return true;
  else
    return draw_view_model_original(me);
}
