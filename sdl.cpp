#include <SDL2/SDL.h>
#include <GL/glew.h>

#include "surface.hpp"

#include "print.hpp"

#include "menu.cpp"

void (*swap_window_original)(void *) = NULL;
int (*poll_event_original)(SDL_Event *) = NULL;


enum input_type
{
    INPUT_NONE,
    INPUT_KEY,
    INPUT_MOUSE,
};

inline static bool menu_focused = false;

int poll_event_hook(SDL_Event *event) {
  int ret = poll_event_original(event);
  nk_sdl_handle_event(event);

  /*
  if (ret && nk_sdl_handle_event(event) && menu_focused) {
    // TBD: Make an array for keybinds
    if (config.aimbot.key.binding.editing) {
      const Uint8 *keyboard_state = SDL_GetKeyboardState(NULL);

      for (int i = 0; i < SDL_NUM_SCANCODES; i++) {
	if (keyboard_state[i]) {
	  if (i == SDL_SCANCODE_ESCAPE) {
	    config.aimbot.key.binding.code = 0;
	    config.aimbot.key.binding.type = INPUT_NONE;
	  } else {
	    config.aimbot.key.binding.code = i;
	    config.aimbot.key.binding.type = INPUT_KEY;
	  }
	  config.aimbot.key.binding.editing = 0;

	  // TBD: Use SDL_GetScancodeName(i) to pretty print the key in menu
	  break;
	}
      }
    }

    if (config.aimbot.key.binding.editing) {
      Uint32 mouse_state = SDL_GetMouseState(NULL, NULL);
            
      for (int i = SDL_BUTTON_LEFT; i <= SDL_BUTTON_X2; i++) {
	if (mouse_state & SDL_BUTTON(i)) {
	  config.aimbot.key.binding.code = i;
	  config.aimbot.key.binding.type = INPUT_MOUSE;
	  config.aimbot.key.binding.editing = 0;
	}
      }
    }

    event->type = 0;

    return ret;
  }

  if (config.aimbot.key.binding.type == INPUT_MOUSE) {
    Uint32 mouse_state = SDL_GetMouseState(NULL, NULL);
    if (mouse_state & SDL_BUTTON(config.aimbot.key.binding.code)) {
      config.aimbot.key.is_pressed = true;
    } else {
      config.aimbot.key.is_pressed = false;
    }
  } else if (config.aimbot.key.binding.type == INPUT_KEY) {
    const Uint8 *keyboard_state = SDL_GetKeyboardState(NULL);
    if (keyboard_state && keyboard_state[config.aimbot.key.binding.code]) {
      config.aimbot.key.is_pressed = true;
    } else {
      config.aimbot.key.is_pressed = false;
    }
  }
  else {
    config.aimbot.key.is_pressed = false;
  }
  */
  
  return ret;
}

void set_style(nk_context* ctx) {
  struct nk_color table[NK_COLOR_COUNT];
  memcpy(table, nk_default_color_style, sizeof(nk_default_color_style));

  table[NK_COLOR_TOGGLE_CURSOR] = nk_rgba(207, 115, 54, 255);
  table[NK_COLOR_TOGGLE_HOVER] = nk_rgba(207, 115, 54, 255 / 2);

  nk_style_from_table(ctx, table);
}


void get_input(nk_context* ctx) {
  if (nk_input_is_key_released(&ctx->input, NK_KEY_DEL)) {
    menu_focused = !menu_focused;
    surface->set_cursor_visible(menu_focused);
  }
}

void watermark(nk_context* ctx) {
  if (nk_begin(ctx, "watermark", nk_rect(10, 10, 150, 30), NK_WINDOW_BORDER | NK_WINDOW_NO_INPUT | NK_WINDOW_NO_SCROLLBAR)) {
    NK_TEXT_ROW(ctx, "I Use Arch BTW!!!", NK_TEXT_CENTERED);
  }

  nk_end(ctx);
}


void swap_window_hook(SDL_Window* window) {
  static struct nk_context* ctx = NULL;
  static SDL_GLContext original_ctx, new_ctx;

  if (!ctx) {
    original_ctx = SDL_GL_GetCurrentContext();
    new_ctx = SDL_GL_CreateContext(window);

    GLenum err = glewInit();
    if (err != GLEW_OK) {
      print("Failed to initialize GLEW: %s\n", glewGetErrorString(err));
      swap_window_original(window);
      return;
    }
    
    ctx = nk_sdl_init(window);

    set_style(ctx);
        
    struct nk_font_atlas* atlas = NULL;
    nk_sdl_font_stash_begin(&atlas);
    nk_sdl_font_stash_end();
  }

  SDL_GL_MakeCurrent(window, new_ctx);
  get_input(ctx);
  watermark(ctx);

  if (menu_focused) {
    draw_menu(ctx);
  }

#define MAX_VERTEX_MEMORY 512 * 1024
#define MAX_ELEMENT_MEMORY 128 * 1024

  nk_sdl_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_MEMORY, MAX_ELEMENT_MEMORY);

  SDL_GL_MakeCurrent(window, original_ctx);

  nk_input_begin(ctx);
  swap_window_original(window);
  nk_input_end(ctx);
}
