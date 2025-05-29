#include <stdlib.h>
 
#include "entity.hpp"

#include "config.hpp"

#include "vec.hpp"

#include "surface.hpp"
#include "debug_overlay.hpp"
#include "render_view.hpp"

extern unsigned long esp_entity_font;

void box_esp_entity(Vec3 screen, Entity* entity) {
  if (entity->get_class_id() == AMMO_OR_HEALTH_PACK && config.esp.pickup.box) {
    surface->set_rgba(255, 255, 255, 255);
    surface->draw_outlined_rect(screen.x-5, screen.y-5, screen.x+5, screen.y+5);
  }
}

void name_esp_entity(Vec3 screen, Entity* entity) {

  if (entity->get_class_id() == AMMO_OR_HEALTH_PACK && config.esp.pickup.name) {
    surface->draw_set_text_color(255, 255, 255, 255);
   
    if (entity->get_pickup_type() == AMMOPACK) {
      surface->draw_set_text_pos(screen.x - (wcslen(L"AMMO")/2 * surface->get_character_width(esp_entity_font, 'A')), screen.y - 17);
      surface->draw_print_text(L"AMMO", wcslen(L"AMMO"));
    } else if (entity->get_pickup_type() == MEDKIT) {
      surface->draw_set_text_pos(screen.x - (wcslen(L"HEALTH")/2 * surface->get_character_width(esp_entity_font, 'A')), screen.y - 17);
      surface->draw_print_text(L"HEALTH", wcslen(L"HEALTH"));
    }
    /*else {
      std::string model_name = entity->get_model_name();
      
      wchar_t model_name_w[64];
      size_t len = mbstowcs(model_name_w, model_name.c_str(), 64);
      if (len == (size_t)-1) return;
      
      surface->draw_set_text_pos(screen.x, screen.y);
      surface->draw_print_text(model_name_w, wcslen(model_name_w));

      }
    */
  }
}

void esp_entity(unsigned int i, Entity* entity) {
  Vec3 location = entity->get_origin();

  Vec3 screen;
  if (!render_view->world_to_screen(&location, &screen)) return;

  box_esp_entity(screen, entity);
  name_esp_entity(screen, entity);
}
