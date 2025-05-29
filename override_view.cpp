#include "vec.hpp"

#include "entity_list.hpp"

#include "config.hpp"

#include "player.hpp"

void (*override_view_original)(void*, view_setup*);


void override_view_hook(void* me, view_setup* setup) {
  override_view_original(me, setup);

  Player* localplayer = entity_list->get_localplayer();

    
  if (config.visuals.remove_zoom == true) {
    setup->fov = config.visuals.override_fov ? config.visuals.custom_fov : localplayer->get_default_fov();
  } else {
    if (config.visuals.override_fov == true && (localplayer->get_cond_flags() & 20) == false)
      setup->fov = config.visuals.custom_fov;
  }

}
