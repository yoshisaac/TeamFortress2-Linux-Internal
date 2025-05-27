#include <stdlib.h>
#include <string>

#include "entity.hpp"

#include "config.hpp"

#include "vec.hpp"

#include "surface.hpp"
#include "debug_overlay.hpp"

void box_esp_entity() {
  
}

void name_esp_entity(Vec3 screen, Entity* entity) {

  if (entity->get_class_id() == AMMO_OR_HEALTH_PACK && config.esp.pickup.name) {
  surface->draw_set_text_color(255, 255, 255, 255);
  surface->draw_set_text_pos(screen.x, screen.y);
  
  if (entity->get_pickup_type() == AMMOPACK)
    surface->draw_print_text(L"AMMO", wcslen(L"AMMO"));
  else if (entity->get_pickup_type() == MEDKIT)
    surface->draw_print_text(L"HEALTH", wcslen(L"HEALTH"));
  }
}

void esp_entity(Entity* entity) {
  Vec3 location = entity->get_origin();

  Vec3 screen;
  if (!overlay->world_to_screen(&location, &screen)) return;

  name_esp_entity(screen, entity);
}
