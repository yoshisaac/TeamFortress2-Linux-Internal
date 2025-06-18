#include "engine.hpp"
#include "entity_list.hpp"

#include "config.hpp"

#include "print.hpp"

#include "aimbot.cpp"

enum in_buttons {
  IN_ATTACK = (1 << 0),
  IN_JUMP = (1 << 1),
  IN_DUCK = (1 << 2),
  IN_FORWARD = (1 << 3),
  IN_BACK = (1 << 4),
  IN_USE = (1 << 5),
  IN_CANCEL = (1 << 6),
  IN_LEFT = (1 << 7),
  IN_RIGHT = (1 << 8),
  IN_MOVELEFT = (1 << 9),
  IN_MOVERIGHT = (1 << 10),
  IN_ATTACK2 = (1 << 11),
  IN_RUN = (1 << 12),
  IN_RELOAD = (1 << 13),
  IN_ALT1 = (1 << 14),
  IN_ALT2 = (1 << 15),
  IN_SCORE = (1 << 16),
  IN_SPEED = (1 << 17),
  IN_WALK = (1 << 18),
  IN_ZOOM = (1 << 19),
  IN_WEAPON1 = (1 << 20),
  IN_WEAPON2 = (1 << 21),
  IN_BULLRUSH = (1 << 22),
  IN_GRENADE1 = (1 << 23),
  IN_GRENADE2 = (1 << 24),
  IN_ATTACK3 = (1 << 25),
};

bool (*create_move_original)(void*, float, user_cmd*);

bool create_move_hook(void* me, float sample_time, user_cmd* user_cmd) {

  /*
  if (!user_cmd->tick_count) {
    return true;
  }
  */
  
  bool rc = create_move_original(me, sample_time, user_cmd);

  if (!engine->is_in_game()) {
    return rc;
  }

  Player* localplayer = entity_list->player_from_index(engine->get_localplayer_index());

  if (!localplayer) {
    print("localplayer is NULL\n");
    return rc;
  }
  
  if (user_cmd->tick_count > 1) {

    static bool was_pressed = false;
    static bool do_thirdperson = false;
    if (config.visuals.thirdperson == true) {
      if (!was_pressed && is_button_down(config.visuals.thirdperson_key)) {
	do_thirdperson = !do_thirdperson;
	localplayer->set_thirdperson(do_thirdperson);
	was_pressed = true;
      } else if (was_pressed && !is_button_down(config.visuals.thirdperson_key)) {
	was_pressed = false;
      }
    } else {
      localplayer->set_thirdperson(false);
    }

    
    aimbot(user_cmd);
    
    static bool was_jumping = false;
    bool on_ground = (localplayer->get_ent_flags() & FL_ONGROUND);

    if(user_cmd->buttons & IN_JUMP && config.misc.bhop == true) {

      if(!was_jumping && !on_ground) {
	user_cmd->buttons &= ~IN_JUMP;
      } else if(was_jumping) {
	was_jumping = false;
      }
      
    } else if(!was_jumping) {
      was_jumping = true;
    }
  }
  
  if (config.aimbot.silent == true)
    return false;
  else
    return rc;
}
