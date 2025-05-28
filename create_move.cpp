#include <cmath>

#include "engine.hpp"
#include "entity_list.hpp"

#include "config.hpp"

#include "vec.hpp"
#include "print.hpp"

#include "aimbot.hpp"

struct user_cmd {
  void *vmt;
  int command_number;
  int tick_count;
  Vec3 view_angles;
  float forwardmove;
  float sidemove;
  float upmove;
  int buttons;
  unsigned char impulse;
  int weapon_select;
  int weapon_subtype;
  int random_seed;
  short mouse_dx;
  short mouse_dy;
  bool has_been_predicted;
};

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

bool (*create_move_original)(void*, float, void*);

void movement_fix(user_cmd* user_cmd, Vec3 original_view_angle, float original_forward_move, float original_side_move) {
  float yaw_delta = user_cmd->view_angles.y - original_view_angle.y;
  float original_yaw_correction = 0;
  float current_yaw_correction = 0;

  if (original_view_angle.y < 0.0f) {
    original_yaw_correction = 360.0f + original_view_angle.y;
  } else {
    original_yaw_correction = original_view_angle.y;
  }
    
  if (user_cmd->view_angles.y < 0.0f) {
    current_yaw_correction = 360.0f + user_cmd->view_angles.y;
  } else {
    current_yaw_correction = user_cmd->view_angles.y;
  }

  if (current_yaw_correction < original_yaw_correction) {
    yaw_delta = abs(current_yaw_correction - original_yaw_correction);
  } else {
    yaw_delta = 360.0f - abs(original_yaw_correction - current_yaw_correction);
  }
  yaw_delta = 360.0f - yaw_delta;

  user_cmd->forwardmove = cos((yaw_delta) * (M_PI/180)) * original_forward_move + cos((yaw_delta + 90.f) * (M_PI/180)) * original_side_move;
  user_cmd->sidemove = sin((yaw_delta) * (M_PI/180)) * original_forward_move + sin((yaw_delta + 90.f) * (M_PI/180)) * original_side_move;
}

bool create_move_hook(void* me, float sample_time, user_cmd* user_cmd) {
  bool rc = create_move_original(me, sample_time, user_cmd);

  Vec3 original_view_angle = user_cmd->view_angles;
  float original_side_move = user_cmd->sidemove;
  float original_forward_move = user_cmd->forwardmove;


  if (!engine->is_in_game()) {
    return rc;
  }

  Player* localplayer = entity_list->player_from_index(engine->get_localplayer_index());

  if (!localplayer) {
    print("localplayer is NULL\n");
    return rc;
  }
  
  if (user_cmd->tick_count > 1) {
    float smallest_fov_angle = __FLT_MAX__;

    for (unsigned int i = 1; i <= entity_list->get_max_entities(); ++i) {
      Player* player = entity_list->player_from_index(i);
      if (player == NULL || 
	  player == localplayer || 
	  player->is_dormant() || 
	  player->get_team() == localplayer->get_team() ||
	  player->get_lifestate() != 1
	  ) {
	continue;
      }

      
      int head_bone = player->get_head_bone();
      
      Vec3 diff = {player->get_bone_pos(head_bone).x - localplayer->get_shoot_pos().x,
		   player->get_bone_pos(head_bone).y - localplayer->get_shoot_pos().y,
		   player->get_bone_pos(head_bone).z - localplayer->get_shoot_pos().z};
      
      float yaw_hyp = sqrt((diff.x * diff.x) + (diff.y * diff.y));

      float pitch_angle = atan2(diff.z, yaw_hyp) * 180 / M_PI;
      float yaw_angle = atan2(diff.y, diff.x) * 180 / M_PI;

      Vec3 view_angles = {
        .x = -pitch_angle,
        .y = yaw_angle,
        .z = 0
      };

      
      float x_diff = view_angles.x - original_view_angle.x;
      float y_diff = view_angles.y - original_view_angle.y;

      float x = remainderf(x_diff, 360.0f);
      float y = remainderf(y_diff, 360.0f);

      float clamped_x = x > 89.0f ? 89.0f : x < -89.0f ? -89.0f : x;
      float clamped_y = y > 180.0f ? 180.0f : y < -180.0f ? -180.0f : y;

      float fov = hypotf(clamped_x, clamped_y);

      if (fov <= config.aimbot.fov && fov < smallest_fov_angle) {
	Aimbot::target_player = player;
	smallest_fov_angle = fov;
      }	

      if (config.aimbot.master == true && (user_cmd->buttons & 1) != 0 && Aimbot::target_player == player)
	user_cmd->view_angles = view_angles;

      movement_fix(user_cmd, original_view_angle, original_forward_move, original_side_move);

      
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
  }
  
  if (config.aimbot.silent == true)
    return false;
  else
    return rc;
}
