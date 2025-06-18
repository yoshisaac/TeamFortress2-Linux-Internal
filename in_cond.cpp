#include "config.hpp"

bool (*in_cond_original)(void*, int);

bool in_cond_hook(void* me, int mask) {

  if (mask == 1 && config.visuals.hide_scope == true) { //if they player is scoped
    return false;
  }

  bool re = in_cond_original(me, mask);
  
  return re;
}
