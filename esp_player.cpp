#include "config.hpp"
#include "vec.hpp"

#include "player.hpp"

#include "math.hpp"

#include "aimbot.hpp"

#include "surface.hpp"
#include "render_view.hpp"
#include "entity_list.hpp"
#include "engine.hpp"
#include "convar_system.hpp"

extern unsigned long esp_player_font;


void box_esp_player(Vec3 screen, Vec3 screen_offset) {
  if (config.esp.player.box == true) {
    
    float box_offset = (screen.y - screen_offset.y)/4;

    /* shadow box */
    surface->set_rgba(0, 0, 0, 255);

    //right side
    surface->draw_line(screen.x + box_offset + 1, screen.y + 1, screen.x + box_offset + 1, screen_offset.y - 1);
    surface->draw_line(screen.x + box_offset - 1, screen.y + 1, screen.x + box_offset - 1, screen_offset.y - 1);
    
    //left side
    surface->draw_line(screen.x - box_offset + 1, screen.y + 1, screen.x - box_offset + 1, screen_offset.y - 1);
    surface->draw_line(screen.x - box_offset - 1, screen.y + 1, screen.x - box_offset - 1, screen_offset.y - 1);
    
    //top
    surface->draw_line(screen.x - box_offset - 1, screen_offset.y + 1, screen.x + box_offset + 2, screen_offset.y + 1);
    surface->draw_line(screen.x - box_offset - 1, screen_offset.y - 1, screen.x + box_offset + 2, screen_offset.y - 1);
    
    //bottom
    surface->draw_line(screen.x - box_offset - 1, screen.y + 1, screen.x + box_offset + 1, screen.y + 1);    
    surface->draw_line(screen.x - box_offset - 1, screen.y - 1, screen.x + box_offset + 1, screen.y - 1);    
    /* shadow box */
    
    /* actual box */
    surface->set_rgba(255, 255, 255, 255);
    
    //right side
    surface->draw_line(screen.x + box_offset, screen.y, screen.x + box_offset, screen_offset.y);

    //left side
    surface->draw_line(screen.x - box_offset, screen.y, screen.x - box_offset, screen_offset.y);

    //top
    surface->draw_line(screen.x - box_offset, screen_offset.y, screen.x + box_offset + 1, screen_offset.y);

    //bottom
    surface->draw_line(screen.x - box_offset, screen.y, screen.x + box_offset, screen.y);    
    /* actual box */
  }

}

void health_bar_esp_player(Vec3 screen, Vec3 screen_offset, Player* player) {
  if (config.esp.player.health_bar == true) {
    float health_offset = (screen.y - screen_offset.y)/4;

    //shadow
    surface->set_rgba(0, 0, 0, 255);
    surface->draw_line(screen.x - health_offset - 5, screen.y + 1, screen.x - health_offset - 5, screen_offset.y - 2);
    surface->draw_line(screen.x - health_offset - 4, screen.y + 1, screen.x - health_offset - 4, screen_offset.y - 2);
    surface->draw_line(screen.x - health_offset - 3, screen.y + 1, screen.x - health_offset - 3, screen_offset.y - 2);

    surface->set_rgba(0, 255, 0, 255);
    int ydelta = (screen_offset.y - screen.y) * (1.f - (float(player->get_health()) / player->get_max_health()));

    if (player->get_health() > player->get_max_health()) { // over healed
      surface->set_rgba(0, 255, 255, 255);
      ydelta = 0;
    }
    else if (player->get_health() <= player->get_max_health() && player->get_health() >= (player->get_max_health()*.9))
      surface->set_rgba(0, 255, 0, 255);
    else if (player->get_health() < (player->get_max_health()*.9) && player->get_health() > (player->get_max_health()*.6))
      surface->set_rgba(90, 255, 0, 255);
    else if (player->get_health() <= (player->get_max_health()*.6) && player->get_health() > (player->get_max_health()*.35))
      surface->set_rgba(255, 100, 0, 255);
    else if (player->get_health() <= (player->get_max_health()*.35))
      surface->set_rgba(255, 0, 0, 255);

  
    surface->draw_line(screen.x - health_offset - 4, screen.y, screen.x - health_offset - 4, screen_offset.y - ydelta - 1);
  }
}

void name_esp_player(Vec3 screen, Vec3 screen_offset, Player* player, unsigned int i) {
  if (config.esp.player.name == true) {
    player_info pinfo;
    if (!engine->get_player_info(i, &pinfo)) return;
  

    wchar_t name[32];
    size_t len = mbstowcs(name, pinfo.name, 32);
    if (len == (size_t)-1) return;

    unsigned int name_length = surface->get_string_width(esp_player_font, name);
    
    surface->draw_set_text_color(255, 255, 255, 255);
    surface->draw_set_text_pos(screen.x - (name_length/2.f) , screen_offset.y - 13);  

    surface->draw_print_text(name, wcslen(name));
  }
}

void flags_esp_player(Vec3 screen, Vec3 screen_offset, Player* player) {
  float flags_x_offset = (screen.y - screen_offset.y)/4;
  float flags_y_offset = 0;
    
  if (player == target_player && config.esp.player.target_indicator == true) {
    surface->draw_set_text_color(255, 0, 0, 255);
    surface->draw_set_text_pos(screen.x + flags_x_offset + surface->get_character_width(esp_player_font, L"TARGET"[0]), screen_offset.y + flags_y_offset);

    surface->draw_print_text(L"TARGET", wcslen(L"TARGET"));

    flags_y_offset += surface->get_font_height(esp_player_font);
  }
}

void skeleton_esp_player(Player* player) {
  //todo
}

void esp_player(unsigned int i, Player* player) {

  bool friendlyfire = false;
  static Convar* friendlyfirevar = convar_system->find_var("mp_friendlyfire");
  if (friendlyfirevar != nullptr) {
    if (friendlyfirevar->get_int() != 0) {
      friendlyfire = true;
    }
  }

  
  Player* localplayer = entity_list->player_from_index(engine->get_localplayer_index());  
  if (player == localplayer ||
      player->is_dormant() ||
      player->get_lifestate() != 1 ||
      (player->get_team() == localplayer->get_team() && friendlyfire == false)) {
    return;
  }

  /*
  //bone draw ID debug
  surface->set_rgba(255, 255, 255, 255);
  for (unsigned int h = 0; h < 128; ++h) {
    Vec3 bone = player->get_bone_pos(h);
    Vec3 bone_screen;
    render_view->world_to_screen(&bone, &bone_screen);
    surface->draw_set_text_pos(bone_screen.x, bone_screen.y);
    std::wstring a = std::to_wstring(h);
    surface->draw_print_text(a.c_str(), wcslen(a.c_str()));
  }
  */
  
  Vec3 location = player->get_origin();
  Vec3 screen;
  if (!render_view->world_to_screen(&location, &screen)) return;

  float distance = distance_3d(localplayer->get_origin(), player->get_origin());
    

  Vec3 z_offset = {location.x, location.y, player->get_bone_pos(player->get_head_bone()).z + 10};
  Vec3 screen_offset;
  render_view->world_to_screen(&z_offset, &screen_offset);

  box_esp_player(screen, screen_offset);
  health_bar_esp_player(screen, screen_offset, player);
  flags_esp_player(screen, screen_offset, player);
  name_esp_player(screen, screen_offset, player, i);
  skeleton_esp_player(player);
}
