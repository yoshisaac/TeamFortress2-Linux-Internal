#include <cmath>

#include "engine.hpp"
#include "entity_list.hpp"

#include "vec.hpp"
#include "print.hpp"


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

bool (*create_move_original)(void*, float, void*);

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
    
    Player* target_player = nullptr;
    float smallest_fov_angle = __FLT_MAX__;

    for (unsigned int i = 1; i <= entity_list->get_max_entities(); ++i) {
      Player* player = entity_list->player_from_index(i);
      if (player == NULL || 
	  player == localplayer || 
	  player->is_dormant() || 
	  player->get_team() == localplayer->get_team() ||
	  player->get_lifestate() != 1) {
	continue;
      }

      
      int head_bone = player->get_head_bone();
      
      Vec3 diff = {player->get_bone_pos(head_bone).x - localplayer->get_shoot_pos().x,
		   player->get_bone_pos(head_bone).y - localplayer->get_shoot_pos().y,
		   player->get_bone_pos(head_bone).z - localplayer->get_shoot_pos().z};
      
      float c = sqrt((diff.x * diff.x) + (diff.y * diff.y));

      float pitch_angle = atan2(diff.z, c) * 180 / M_PI;
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

      if (fov <= 45 && fov < smallest_fov_angle) {
	target_player = player;
	smallest_fov_angle = fov;
      } else {
	target_player = nullptr;
      }
      
      if ((user_cmd->buttons & 1) != 0 && target_player != nullptr) 
	user_cmd->view_angles = view_angles;

    }
  }

  return rc;
}
