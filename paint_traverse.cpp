#include <cstring>
#include <unistd.h>
#include <wchar.h>
#include <string>

#include "engine.hpp"

#include "player.hpp"
#include "surface.hpp"
#include "debug_overlay.hpp"
#include "entity_list.hpp"

#include "config.hpp"

#include "math.hpp"
#include "print.hpp"

#include "esp_player.cpp"
#include "esp_entity.cpp"

void (*paint_traverse_original)(void*, void*, __int8_t, __int8_t) = NULL;

void* vgui;
const char* get_panel_name(void* panel) {
    void **vtable = *(void ***)vgui;

    const char* (*get_panel_name_fn)(void*, void*) = (const char* (*)(void*, void*))vtable[37];

    return get_panel_name_fn(vgui, panel);
}


void paint_traverse_hook(void* me, void* panel, __int8_t force_repaint, __int8_t allow_force) {
  std::string panel_name = get_panel_name(panel);

  paint_traverse_original(me, panel, force_repaint, allow_force);

  //print("%s\n", panel_name.c_str());
  
  if (panel_name != "MatSystemTopPanel") {
    return;
  }

  if (!engine->is_in_game()) {
    return;
  }

  unsigned long esp_font = surface->text_create_font();
  surface->text_set_font_glyph_set(esp_font, "ProggySquare", 14, 400, 0, 0, 0x0);

  
  for (unsigned int i = 1; i <= entity_list->get_max_entities(); ++i) {
    if (config.esp.master == false) continue;

    Player* player = entity_list->player_from_index(i);
    if (player == nullptr) continue;

    if (player->get_class_name() == "PLAYER")
      esp_player(player);
    else
      esp_entity(player->to_entity());
  }
}
