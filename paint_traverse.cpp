#include <cstring>
#include <unistd.h>
#include <wchar.h>

#include "engine.hpp"

#include "player.hpp"
#include "surface.hpp"
#include "debug_overlay.hpp"
#include "entity_list.hpp"

#include "math.hpp"

void (*paint_traverse_original)(void*, void*, __int8_t, __int8_t) = NULL;

void* vgui;
const char* get_panel_name(void* panel) {
    void **vtable = *(void ***)vgui;

    const char* (*get_panel_name_fn)(void*, void*) = (const char* (*)(void*, void*))vtable[37];

    return get_panel_name_fn(vgui, panel);
}


void paint_traverse_hook(void* me, void* panel, __int8_t force_repaint, __int8_t allow_force) {
  const char* panel_name = get_panel_name(panel);

  paint_traverse_original(me, panel, force_repaint, allow_force);


  if (strcmp(panel_name, "FocusOverlayPanel") != 0) {
    return;
  }

  if (!engine->is_in_game()) {
    return;
  }

  
  Player* localplayer = entity_list->player_from_index(engine->get_localplayer_index());
  
  for (unsigned int i = 1; i <= entity_list->get_max_entities(); ++i) {
    Player* player = entity_list->player_from_index(i);

    if (player == NULL || player == localplayer) {
      continue;
    }

    
    if (player->is_dormant() || player->get_lifestate() != 1 || player->get_team() == localplayer->get_team()) {
      continue;
    }

    Vec3 location = player->get_origin();
    Vec3 screen;
    if (!overlay->world_to_screen(&location, &screen)) continue;

    float distance = distance_3d(localplayer->get_origin(), player->get_origin());
    
    surface->set_rgba(255, 255, 255, 255);

    Vec3 z_offset = {location.x, location.y, location.z + 82};
    Vec3 screen_offset;
    overlay->world_to_screen(&z_offset, &screen_offset);

    /* bone draw ID debug
    for (unsigned int h = 0; h < 128; ++h) {
      Vec3 bone = player->get_bone_pos(h);
      Vec3 bone_screen;
      overlay->world_to_screen(&bone, &bone_screen);
      surface->draw_set_text_pos(bone_screen.x, bone_screen.y);
      std::wstring a = std::to_wstring(h);
      surface->draw_print_text(a.c_str(), wcslen(a.c_str()));
    }
    */
    
    float box_offset = (13000/distance) * (float(localplayer->get_default_fov())/float(localplayer->get_fov()));
    
    //right side
    surface->draw_line(screen.x + box_offset, screen.y, screen.x + box_offset, screen_offset.y);

    //left side
    surface->draw_line(screen.x - box_offset, screen.y, screen.x - box_offset, screen_offset.y);

    //top
    surface->draw_line(screen.x - box_offset, screen_offset.y, screen.x + box_offset, screen_offset.y);

    //bottom
    surface->draw_line(screen.x - box_offset, screen.y, screen.x + box_offset, screen.y);    
    
  }
}
