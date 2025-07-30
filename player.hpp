#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "entity_list.hpp"

#include "entity.hpp"

#include "weapon.hpp"

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

struct player_info {
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

enum {
  TF_COND_INVALID                          = -1,
  TF_COND_AIMING                           = 0, // Sniper aiming, Heavy minigun.
  TF_COND_ZOOMED                           = 1,
  TF_COND_DISGUISING                       = 2,
  TF_COND_DISGUISED                        = 3,
  TF_COND_STEALTHED                        = 4, // Spy specific
  TF_COND_INVULNERABLE                     = 5,
  TF_COND_TELEPORTED                       = 6,
  TF_COND_TAUNTING                         = 7,
  TF_COND_INVULNERABLE_WEARINGOFF          = 8,
  TF_COND_STEALTHED_BLINK                  = 9,
  TF_COND_SELECTED_TO_TELEPORT             = 10,
  TF_COND_CRITBOOSTED                      = 11, // DO NOT RE-USE THIS -- THIS IS FOR KRITZKRIEG AND REVENGE CRITS ONLY
  TF_COND_TMPDAMAGEBONUS                   = 12,
  TF_COND_FEIGN_DEATH                      = 13,
  TF_COND_PHASE                            = 14,
  TF_COND_STUNNED                          = 15, // Any type of stun. Check iStunFlags for more info.
  TF_COND_OFFENSEBUFF                      = 16,
  TF_COND_SHIELD_CHARGE                    = 17,
  TF_COND_DEMO_BUFF                        = 18,
  TF_COND_ENERGY_BUFF                      = 19,
  TF_COND_RADIUSHEAL                       = 20,
  TF_COND_HEALTH_BUFF                      = 21,
  TF_COND_BURNING                          = 22,
  TF_COND_HEALTH_OVERHEALED                = 23,
  TF_COND_URINE                            = 24,
  TF_COND_BLEEDING                         = 25,
  TF_COND_DEFENSEBUFF                      = 26, // 35% defense! No crit damage.
  TF_COND_MAD_MILK                         = 27,
  TF_COND_MEGAHEAL                         = 28,
  TF_COND_REGENONDAMAGEBUFF                = 29,
  TF_COND_MARKEDFORDEATH                   = 30,
  TF_COND_NOHEALINGDAMAGEBUFF              = 31,
  TF_COND_SPEED_BOOST                      = 32, // = 32
  TF_COND_CRITBOOSTED_PUMPKIN              = 33, // Brandon hates bits
  TF_COND_CRITBOOSTED_USER_BUFF            = 34,
  TF_COND_CRITBOOSTED_DEMO_CHARGE          = 35,
  TF_COND_SODAPOPPER_HYPE                  = 36,
  TF_COND_CRITBOOSTED_FIRST_BLOOD          = 37, // arena mode first blood
  TF_COND_CRITBOOSTED_BONUS_TIME           = 38,
  TF_COND_CRITBOOSTED_CTF_CAPTURE          = 39,
  TF_COND_CRITBOOSTED_ON_KILL              = 40, // =40. KGB, etc.
  TF_COND_CANNOT_SWITCH_FROM_MELEE         = 41,
  TF_COND_DEFENSEBUFF_NO_CRIT_BLOCK        = 42, // 35% defense! Still damaged by crits.
  TF_COND_REPROGRAMMED                     = 43, // Bots only
  TF_COND_CRITBOOSTED_RAGE_BUFF            = 44,
  TF_COND_DEFENSEBUFF_HIGH                 = 45, // 75% defense! Still damaged by crits.
  TF_COND_SNIPERCHARGE_RAGE_BUFF           = 46, // Sniper Rage - Charge time speed up
  TF_COND_DISGUISE_WEARINGOFF              = 47, // Applied for half-second post-disguise
  TF_COND_MARKEDFORDEATH_SILENT            = 48, // Sans sound
  TF_COND_DISGUISED_AS_DISPENSER           = 49,
  TF_COND_SAPPED                           = 50, // =50. Bots only
  TF_COND_INVULNERABLE_HIDE_UNLESS_DAMAGED = 51,
  TF_COND_INVULNERABLE_USER_BUFF           = 52,
  TF_COND_HALLOWEEN_BOMB_HEAD              = 53,
  TF_COND_HALLOWEEN_THRILLER               = 54,
  TF_COND_RADIUSHEAL_ON_DAMAGE             = 55,
  TF_COND_CRITBOOSTED_CARD_EFFECT          = 56,
  TF_COND_INVULNERABLE_CARD_EFFECT         = 57,
  TF_COND_MEDIGUN_UBER_BULLET_RESIST       = 58,
  TF_COND_MEDIGUN_UBER_BLAST_RESIST        = 59,
  TF_COND_MEDIGUN_UBER_FIRE_RESIST         = 60, // =60
  TF_COND_MEDIGUN_SMALL_BULLET_RESIST      = 61,
  TF_COND_MEDIGUN_SMALL_BLAST_RESIST       = 62,
  TF_COND_MEDIGUN_SMALL_FIRE_RESIST        = 63,
  TF_COND_STEALTHED_USER_BUFF              = 64, // Any class can have this
  TF_COND_MEDIGUN_DEBUFF                   = 65,
  TF_COND_STEALTHED_USER_BUFF_FADING       = 66,
  TF_COND_BULLET_IMMUNE                    = 67,
  TF_COND_BLAST_IMMUNE                     = 68,
  TF_COND_FIRE_IMMUNE                      = 69,
  TF_COND_PREVENT_DEATH                    = 70, // =70
  TF_COND_MVM_BOT_STUN_RADIOWAVE           = 71, // Bots only
  TF_COND_HALLOWEEN_SPEED_BOOST            = 72,
  TF_COND_HALLOWEEN_QUICK_HEAL             = 73,
  TF_COND_HALLOWEEN_GIANT                  = 74,
  TF_COND_HALLOWEEN_TINY                   = 75,
  TF_COND_HALLOWEEN_IN_HELL                = 76,
  TF_COND_HALLOWEEN_GHOST_MODE             = 77, // =77
  TF_COND_MINICRITBOOSTED_ON_KILL          = 78,
  TF_COND_OBSCURED_SMOKE                   = 79,
  TF_COND_PARACHUTE_ACTIVE                 = 80, // actively being used (not retracted)
  TF_COND_BLASTJUMPING                     = 81,
  TF_COND_HALLOWEEN_KART                   = 82,
  TF_COND_HALLOWEEN_KART_DASH              = 83,
  TF_COND_BALLOON_HEAD                     = 84, // =84 larger head, lower-gravity-feeling jumps
  TF_COND_MELEE_ONLY                       = 85, // =85 melee only
  TF_COND_SWIMMING_CURSE                   = 86, // player movement become swimming movement
  TF_COND_FREEZE_INPUT                     = 87, // freezes player input
  TF_COND_HALLOWEEN_KART_CAGE              = 88, // attach cage model to player while in kart
  TF_COND_DONOTUSE_0                       = 89,
  TF_COND_RUNE_STRENGTH                    = 90,
  TF_COND_RUNE_HASTE                       = 91,
  TF_COND_RUNE_REGEN                       = 92,
  TF_COND_RUNE_RESIST                      = 93,
  TF_COND_RUNE_VAMPIRE                     = 94,
  TF_COND_RUNE_REFLECT                     = 95,
  TF_COND_RUNE_PRECISION                   = 96,
  TF_COND_RUNE_AGILITY                     = 97,
  TF_COND_GRAPPLINGHOOK                    = 98,
  TF_COND_GRAPPLINGHOOK_SAFEFALL           = 99,
  TF_COND_GRAPPLINGHOOK_LATCHED            = 100,
  TF_COND_GRAPPLINGHOOK_BLEEDING           = 101,
  TF_COND_AFTERBURN_IMMUNE                 = 102,
  TF_COND_RUNE_KNOCKOUT                    = 103,
  TF_COND_RUNE_IMBALANCE                   = 104,
  TF_COND_CRITBOOSTED_RUNE_TEMP            = 105,
  TF_COND_PASSTIME_INTERCEPTION            = 106,
  TF_COND_SWIMMING_NO_EFFECTS              = 107, // =107_DNOC_FT
  TF_COND_PURGATORY                        = 108,
  TF_COND_RUNE_KING                        = 109,
  TF_COND_RUNE_PLAGUE                      = 110,
  TF_COND_RUNE_SUPERNOVA                   = 111,
  TF_COND_PLAGUE                           = 112,
  TF_COND_KING_BUFFED                      = 113,
  TF_COND_TEAM_GLOWS                       = 114, // used to show team glows to living players
  TF_COND_KNOCKED_INTO_AIR                 = 115,
  TF_COND_COMPETITIVE_WINNER               = 116,
  TF_COND_COMPETITIVE_LOSER                = 117,
  TF_COND_HEALING_DEBUFF                   = 118,
  TF_COND_PASSTIME_PENALTY_DEBUFF          = 119, // when carrying the ball without any teammates nearby
  TF_COND_GRAPPLED_TO_PLAYER               = 120,
  TF_COND_GRAPPLED_BY_PLAYER               = 121,
  TF_COND_PARACHUTE_DEPLOYED               = 122, // activated at least once while player's been airborne, but not does mean it's active now (see TF_COND_PARACHUTE_ACTIVE)
  TF_COND_GAS                              = 123,
  TF_COND_BURNING_PYRO                     = 124,
  TF_COND_ROCKETPACK                       = 125,
  // Players who lose their footing have lessened friction and don't re-stick to the ground unless they're below a
  // tf_movement_lost_footing_restick speed
  TF_COND_LOST_FOOTING                     = 126,
  // When in the air, slide up/along surfaces with momentum as if caught up in a... blast of air of some sort.
  // Reduces air control as well.  See tf_movement_aircurrent convars.  Removed upon touching ground.
  TF_COND_AIR_CURRENT                      = 127,
  TF_COND_HALLOWEEN_HELL_HEAL              = 128,
  TF_COND_POWERUPMODE_DOMINANT		   = 129,
  TF_COND_IMMUNE_TO_PUSHBACK		   = 130,
  TF_COND_LAST
};

//Original of a hooked class function
static bool (*in_cond_original)(void*, int);

//TODO/NOTE: Something useful would be a network variable helper.
//           There are a lot of magical offsets to specific structures, and those offsets have a string key associated with them.

class Player : public Entity {
public:

  int get_weapon_handle(void) {
    return *(int*)(this + 0x11D0);
  }

  Weapon* get_weapon(void) {
    return (Weapon*)entity_list->entity_from_handle(get_weapon_handle());
  }
   
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
    void** vtable = *(void ***)this;

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
    switch (this->get_class()) {
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
    void** vtable = *(void ***)this;

    int (*setup_bones_fn)(void*, void*, int, int, float) = (int (*)(void*, void*, int, int, float))vtable[96];

    return setup_bones_fn(this, bone_to_world_out, max_bones, bone_mask, current_time);
  }
  
  void set_thirdperson(bool value) {
    *(bool*)(this + 0x240C) = value;
  }

  bool is_thirdperson(void) {
    return *(bool*)(this + 0x240C);
  }  
  
  void* get_shared(void) {
    return (void*)(this + 0x1E78);
  }

  bool in_cond(int condition) {
    return in_cond_original(get_shared(), condition);
  }
  
  bool is_scoped(void) {
    return in_cond_original(get_shared(), TF_COND_ZOOMED);
  }

  int get_tickbase(void) {
    return *(int*)(this + 0x1718);
  }
  
  bool can_shoot(void) {
    Weapon* weapon = this->get_weapon();
    if (!weapon)
      return false;
    
    if (this->get_class() == CLASS_HEAVYWEAPONS)
      return true;

    float cd = weapon->get_next_attack();
    float time = this->get_tickbase() * 0.015f; // i'm gonna explode - rosne
    
    return (cd <= time);
  }
  
  Entity* to_entity(void) {
    return (Entity*)this;
  }
};

#endif

