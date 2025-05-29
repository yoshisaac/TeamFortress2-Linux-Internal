#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "vec.hpp"

class Client {
public:
  bool get_player_view(view_setup& player_view) {
    void** vtable = *(void ***)this;

    bool (*get_player_view_fn)(void*, view_setup&) = (bool (*)(void*, view_setup&))vtable[59];

    return get_player_view_fn(this, player_view);
  }

  float get_fov(void) {
    view_setup local_view = {};
    get_player_view(local_view);

    return local_view.fov;
  }
};

inline static Client* client;

#endif
