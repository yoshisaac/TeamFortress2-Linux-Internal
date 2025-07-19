#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_scancode.h>
#include <cstdint>
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


enum input_type {
  INPUT_NONE,
  INPUT_KEY,
  INPUT_MOUSE,
};

struct button {
  int button;
  input_type button_type;
};

struct Aim {
  nk_bool master = false;

  nk_bool auto_shoot = true;
  
  nk_bool silent = true;
  
  struct button key = {.button = SDL_BUTTON_X1, .button_type = INPUT_MOUSE};
  nk_bool use_key = true;
  
  float fov = 45;
  nk_bool draw_fov = false;
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

struct Visuals {
  nk_bool hide_scope = false;
  nk_bool remove_zoom = false;

  struct button thirdperson_key = {.button = SDL_SCANCODE_LALT, .button_type = INPUT_KEY};
  nk_bool thirdperson = false;
  
  nk_bool override_fov = false;
  float custom_fov = 90;
};

struct Misc {
  nk_bool bhop = true;
  nk_bool bypasspure = true;
  nk_bool no_push = false;
};

struct Config {
  Aim aimbot;
  Esp esp;
  Visuals visuals;
  Misc misc;
};

inline static Config config;


static bool is_button_down(struct button button) {
  if (button.button_type == INPUT_KEY) {
  
    const uint8_t* keys = SDL_GetKeyboardState(NULL);
  
    if (keys[button.button] == 1) {
      return true;
    }

    return false;
  } else if (button.button_type == INPUT_MOUSE) {
    Uint32 mouse_state = SDL_GetMouseState(NULL, NULL);

    if (mouse_state & SDL_BUTTON(button.button))
      return true;

    return false;

  }  

  return false;
}

static void get_button_down(struct button* button) {
  const Uint8* keyboard_state = SDL_GetKeyboardState(NULL);
  for (int i = 0; i < SDL_NUM_SCANCODES; i++) {
    if (keyboard_state[i]) {
      button->button = i;
      button->button_type = INPUT_KEY;	
      break;
    }
  }
    
  Uint32 mouse_state = SDL_GetMouseState(NULL, NULL);            
  for (int i = SDL_BUTTON_MIDDLE; i <= SDL_BUTTON_X2; i++) {
    if (mouse_state & SDL_BUTTON(i)) {
      button->button = i;
      button->button_type = INPUT_MOUSE;
      break;
    }
  }
}

#endif
