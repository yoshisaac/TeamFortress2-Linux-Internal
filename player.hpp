#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "entity.hpp"

#include "vec.hpp"

enum {
  CLASS_UNDEFINED = 0,
  CLASS_SCOUT,
  CLASS_SNIPER,
  CLASS_SOLDIER,
  CLASS_DEMOMAN,
  CLASS_MEDIC,
  CLASS_HEAVYWEAPONS,
  CLASS_PYRO,
  CLASS_SPY,
  CLASS_ENGINEER
};

struct player_info
{
	char name[32];
	int user_id;
	char guid[33];
	unsigned long friends_id;
	char friends_name[32];
	bool fakeplayer;
	bool ishltv;
	unsigned long custom_files[4];
	unsigned char files_downloaded;
};

enum in_cond {
  IN_ZOOMED = (1<<5),
};

class Player : public Entity {
public:
  int get_health(void) {
    return *(int*)(this + 0xD4);
  }
  
  int get_max_health(void) {
    return *(int*)(this + 0x1df8);
  }

  int get_default_fov(void) {
    return *(int*)(this + 0x15E4);
  }

  int get_fov(void) {
    return *(int*)(this + 0x15DC);
  }

  void set_head_size(float size) {
    *(float*)(this + 0x39D8) = size;
  }

  void set_torso_length(float length) {
    *(float*)(this + 0x39DC) = length;
  }

  void set_taunt_cam(bool value) {
    *(bool*)(this + 0x2414) = value;
  }
  
  bool get_lifestate(void) {
    return *(bool*)(this + 0x746);
  }
  
  int get_team(void)  {
    return *(int*)(this + 0xDC);
  }
  
  Vec3 get_shoot_pos(void) {
    void **vtable = *(void ***)this;

    Vec3 (*get_shoot_pos_fn)(void*) = (Vec3 (*)(void*))vtable[303];

    return get_shoot_pos_fn(this);
  }

  int get_class(void) {
    return *(int*)(this + 0x1BA0);
  }

  Vec3 get_bone_pos(int bone_num) {
    // 128 bones, 3x4 matrix
    float bone_to_world_out[128][3][4];
    if (this->setup_bones(bone_to_world_out, 128, 0x100, 0.0f)) {
      // Saw this in the source leak, don't know how it works
      return (Vec3){bone_to_world_out[bone_num][0][3], bone_to_world_out[bone_num][1][3], bone_to_world_out[bone_num][2][3]};
    }

    return (Vec3){0.0f, 0.0f, 0.0f};
  }

  
  int get_head_bone(void) {
    int ent_class = this->get_class();

    switch (ent_class) {
    case CLASS_SCOUT:
    case CLASS_PYRO:
    case CLASS_SPY:
    case CLASS_MEDIC:
    case CLASS_HEAVYWEAPONS:
    case CLASS_SNIPER:
    case CLASS_SOLDIER:
      return 6;
    case CLASS_DEMOMAN:
      return 16;
    case CLASS_ENGINEER:
      return 8;
    }

    return 0;
  }
    
  int setup_bones(void *bone_to_world_out, int max_bones, int bone_mask, float current_time) {
    void **vtable = *(void ***)this;

    int (*setup_bones_fn)(void*, void*, int, int, float) = (int (*)(void*, void*, int, int, float))vtable[96];

    return setup_bones_fn(this, bone_to_world_out, max_bones, bone_mask, current_time);
  }

  int get_cond_flags(void) {
    return *(int*)(this + 0x15D8);
  }
  
  Entity* to_entity(void) {
    return (Entity*)this;
  }
};

#endif
