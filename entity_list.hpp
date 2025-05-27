#ifndef PLAYER_LIST_HPP
#define PLAYER_LIST_HPP


#include "entity.hpp"
#include "player.hpp"

class EntityList {
public:
  Entity* entity_from_index(unsigned int index) {
    void **vtable = *(void ***)this;
    
    Entity* (*get_client_entity_fn)(void*, unsigned int) = (Entity* (*)(void*, unsigned int))vtable[3];
    
    return get_client_entity_fn(this, index);
  }

  Player* player_from_index(unsigned int index) {
    return (Player*)entity_from_index(index);
  }
  
  int get_max_entities(void) {
    void **vtable = *(void ***)this;
    
    int (*get_max_entities_fn)(void*) = (int (*)(void*))vtable[8];

    return get_max_entities_fn(this);
  }
};

static inline EntityList* entity_list;

#endif
