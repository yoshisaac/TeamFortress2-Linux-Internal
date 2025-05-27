#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <string>

#include "vec.hpp"

#include "print.hpp"

enum class_id {
  AMMO_OR_HEALTH_PACK = 1,
  DISPENSER = 86,
  SENTRY = 88,
  TELEPORTER = 89,
  ARROW = 122,
  PLAYER = 247,
  ROCKET = 264,
  PILL_OR_STICKY = 217,
  FLARE = 257,
  CROSSBOW_BOLT = 259,
};

enum pickup_type {
  UNKNOWN = 0,
  AMMOPACK,
  MEDKIT
};

class Entity {
public:  
  Vec3 get_origin(void) {
    // x + 0x328, y + 0x332, z + 0x346
    return *(Vec3*)(this + 0x328);
  }
  
  int get_ent_flags(void) {
    return *(int*)(this + 0x460);
  }
  
  // TODO: Substitute "void*" with real class type
  void* get_networkable(void) {
    return (void*)(this + 0x10);
  }

  void* get_renderable(void) {
    return (void*)(this + 0x8);
  }


  // model_t* = entity + 0x88
  // model_name* = model_t + 0x8 
  
  const char* get_model_name(void) {
    return (const char*)*(unsigned long*)(*(unsigned long*)(this + 0x88) + 0x8);
  }

  enum pickup_type get_pickup_type(void) {
    std::string model_name = get_model_name();
    
    if (model_name == "models/items/ammopack_large.mdl"       || 
	model_name == "models/items/ammopack_medium.mdl"      ||
	model_name == "models/items/ammopack_small.mdl"       ||
	model_name == "models/items/ammopack_large_bday.mdl"  ||
	model_name == "models/items/ammopack_medium_bday.mdl" ||
	model_name == "models/items/ammopack_small_bday.mdl"   ) {
      return pickup_type::AMMOPACK;
    }

    if (model_name == "models/items/medkit_large.mdl"       || 
	model_name == "models/items/medkit_medium.mdl"      ||
	model_name == "models/items/medkit_small.mdl"       ||
	model_name == "models/items/medkit_large_bday.mdl"  ||
	model_name == "models/items/medkit_medium_bday.mdl" ||
	model_name == "models/items/medkit_small_bday.mdl"   ) {
      return pickup_type::MEDKIT;
    }

    return pickup_type::UNKNOWN;
  }
  
  bool is_dormant(void) {
    void* networkable = get_networkable();
    void** vtable = *(void ***)networkable;

    bool (*is_dormant_fn)(void*) = (bool (*)(void*))vtable[8];

    return is_dormant_fn(networkable);
  }  
  
  void* get_client_class(void) {
    void* networkable = get_networkable();
    void** vtable = *(void ***)networkable;

    void* (*get_client_class_fn)(void*) = (void* (*)(void*))vtable[2];

    return get_client_class_fn(networkable);
  }
  
  int get_class_id(void) {
    void* client_class = get_client_class();
    return *(int*)((unsigned long)(client_class) + 0x28);
  }

  
  std::string get_class_name(void) {
    // TBD: Check model name for more accurate class name
    switch (get_class_id()) {
    case AMMO_OR_HEALTH_PACK:
      return "AMMO/HP";
    case DISPENSER:
      return "DISPENSER";
    case SENTRY:
      return "SENTRY";
    case TELEPORTER:
      return "TELEPORTER";
    case ARROW:
      return "ARROW";
    case PLAYER:
      return "PLAYER";
    case ROCKET:
      return "ROCKET";
    case PILL_OR_STICKY:
      return "NADE";
    case FLARE:
      return "FLARE";
    case CROSSBOW_BOLT:
      return "BOLT";
    default:
      return "NOT IMPLEMENTED";
    }
  }

};

#endif
