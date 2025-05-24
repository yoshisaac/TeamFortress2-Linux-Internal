#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "vec.hpp"

class Engine {
public:
  int get_localplayer_index(void) {
    void** vtable = *(void ***)this;
    int (*get_localplayer_index_fn)(void*) = (int (*)(void*))vtable[12];
    
    return get_localplayer_index_fn(this);
  }

  
  Vec2 get_screen_size(void) {
    void** vtable = *(void ***)this;

    void (*get_screen_size_fn)(void*, int*, int*) = (void (*)(void*, int*, int*))vtable[5];
    
    int width;
    int height;
    
    get_screen_size_fn(this, &width, &height);

    return Vec2{width, height};
  }

  bool is_in_game(void) {
    void** vtable = *(void ***)this;
    bool (*is_in_game_fn)(void*) = (bool (*)(void*))vtable[26];

    return is_in_game_fn(this);
  }
};

static inline Engine* engine;


#endif
