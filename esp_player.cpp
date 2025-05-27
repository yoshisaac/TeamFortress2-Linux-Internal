#include "config.hpp"
#include "vec.hpp"

#include "player.hpp"

#include "print.hpp"
#include "math.hpp"

#include "surface.hpp"
#include "debug_overlay.hpp"
#include "entity_list.hpp"
#include "engine.hpp"

void box_esp_player(Vec3 screen, Vec3 screen_offset) {
  if (config.esp.player.box == true) {
    surface->set_rgba(255, 255, 255, 255);
    
    float box_offset = (screen.y - screen_offset.y)/4;
      
    //right side
    surface->draw_line(screen.x + box_offset, screen.y, screen.x + box_offset, screen_offset.y);

    //left side
    surface->draw_line(screen.x - box_offset, screen.y, screen.x - box_offset, screen_offset.y);

    //top
    surface->draw_line(screen.x - box_offset, screen_offset.y, screen.x + box_offset + 1, screen_offset.y);

    //bottom
    surface->draw_line(screen.x - box_offset, screen.y, screen.x + box_offset, screen.y);    
  }

}

void health_bar_esp_player(Vec3 screen, Vec3 screen_offset, Player* player) {
  if (config.esp.player.health_bar == true) {
    int ydelta = (screen_offset.y - screen.y) * (1.f - (float(player->get_health()) / player->get_max_health()));
    surface->set_rgba(0, 255, 0, 255);
    float health_offset = (screen.y - screen_offset.y)/4;
    
    if (player->get_health() > player->get_max_health()) { // over healed
      surface->set_rgba(0, 255, 255, 255);
      ydelta = 0;
    }
    else if (player->get_health() <= player->get_max_health() && player->get_health() >= (player->get_max_health()*.9))
      surface->set_rgba(0, 255, 0, 255);
    else if (player->get_health() < (player->get_max_health()*.9) && player->get_health() > (player->get_max_health()*.6))
      surface->set_rgba(90, 210, 0, 255);
    else if (player->get_health() <= (player->get_max_health()*.6) && player->get_health() > (player->get_max_health()*.35))
      surface->set_rgba(190, 100, 0, 255);
    else if (player->get_health() <= (player->get_max_health()*.35))
      surface->set_rgba(255, 0, 0, 255);

  
    surface->draw_line(screen.x - health_offset - 5, screen.y, screen.x - health_offset - 5, screen_offset.y - ydelta);
  }
}

void esp_player(Player* player) {
  Player* localplayer = entity_list->player_from_index(engine->get_localplayer_index());
    
  if (player == localplayer ||
      player->is_dormant() ||
      player->get_lifestate() != 1 ||
      player->get_team() == localplayer->get_team()) {
    return;
  }

  Vec3 location = player->get_origin();
  Vec3 screen;
  if (!overlay->world_to_screen(&location, &screen)) return;

  float distance = distance_3d(localplayer->get_origin(), player->get_origin());
    

  Vec3 z_offset = {location.x, location.y, player->get_bone_pos(player->get_head_bone()).z + 10};
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


  box_esp_player(screen, screen_offset);
  health_bar_esp_player(screen, screen_offset, player);
}
