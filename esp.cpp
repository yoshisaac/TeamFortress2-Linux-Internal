#include "config.hpp"
#include "vec.hpp"

#include "player.hpp"

#include "surface.hpp"

void box_esp(Vec3 screen, Vec3 screen_offset) {
  if (config.esp.box == true) {
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

void health_bar_esp(Vec3 screen, Vec3 screen_offset, Player* player) {
  if (config.esp.health_bar == true) {
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
