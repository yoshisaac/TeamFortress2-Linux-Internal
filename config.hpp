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

struct Aimbot {
  nk_bool master = false;
};

struct Esp {
  nk_bool master = true;
  nk_bool box = true;
  nk_bool health_bar = true;
};

struct Config {
  Aimbot aimbot;
  Esp esp;
};

inline static Config config;

#endif
