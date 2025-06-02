#include "player.hpp"

bool (*draw_local_player_original)(void*, Player*);

bool draw_local_player_hook(void* me, Player* player) {  
  return draw_local_player_original(me, player);
}
