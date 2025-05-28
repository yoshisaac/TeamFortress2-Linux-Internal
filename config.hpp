#ifndef CONFIG_HPP
#define CONFIG_HPP

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_SDL_GL3_IMPLEMENTATION
#include "nuklear/nuklear.h"
#include "nuklear/nuklear_sdl_gl3.h"

struct Aim {
  nk_bool master = false;

  nk_bool silent = true;

  float fov = 45;
};

struct Esp {
  nk_bool master = true;

  struct Player {
    nk_bool box = true;
    nk_bool health_bar = true;    
    nk_bool name = true;
    nk_bool target_indicator = true;
  };
  Player player;

  struct Pickup {
    nk_bool box = false;    
    nk_bool name = true;
  };
  Pickup pickup;
};

struct Misc {
  nk_bool bhop = true;
};

struct Config {
  Aim aimbot;
  Esp esp;
  Misc misc;
};

inline static Config config;

#endif
