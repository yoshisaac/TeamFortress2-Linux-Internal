#include <SDL2/SDL.h>
#include <GL/glew.h>

#include "config.hpp"
#include "surface.hpp"

#include "print.hpp"

#include "menu.cpp"

void (*swap_window_original)(void *) = NULL;
int (*poll_event_original)(SDL_Event *) = NULL;

int poll_event_hook(SDL_Event *event) {
  int ret = poll_event_original(event);
  nk_sdl_handle_event(event);

  /*
  if (ret && nk_sdl_handle_event(event)) {
    
    const Uint8* keyboard_state = SDL_GetKeyboardState(NULL);
    
    for (int i = 0; i < SDL_NUM_SCANCODES; i++) {

      if (keyboard_state[i]) {
	
      }
    }

    Uint32 mouse_state = SDL_GetMouseState(NULL, NULL);            
    for (int i = SDL_BUTTON_LEFT; i <= SDL_BUTTON_X2; i++) {
      if (mouse_state & SDL_BUTTON(i)) {

      }
    }

    return ret;
  }
  */
  
  return ret;
}

void watermark(nk_context* ctx) {
  if (nk_begin(ctx, "watermark", nk_rect(10, 10, 150, 30), NK_WINDOW_BORDER | NK_WINDOW_NO_INPUT | NK_WINDOW_NO_SCROLLBAR)) {
    NK_TEXT_ROW(ctx, "I Use Arch BTW!!!", NK_TEXT_CENTERED);
  }

  nk_end(ctx);
}


void swap_window_hook(SDL_Window* window) {
  static struct nk_context* context = NULL;
  static SDL_GLContext original_context, new_context;

  if (!context) {
    original_context = SDL_GL_GetCurrentContext();
    new_context = SDL_GL_CreateContext(window);

    GLenum err = glewInit();
    if (err != GLEW_OK) {
      print("Failed to initialize GLEW: %s\n", glewGetErrorString(err));
      swap_window_original(window);
      return;
    }
    
    context = nk_sdl_init(window);

    set_style(context);
        
    struct nk_font_atlas* atlas = NULL;
    nk_sdl_font_stash_begin(&atlas);
    nk_sdl_font_stash_end();
  }

  SDL_GL_MakeCurrent(window, new_context);
  get_input(context);
  watermark(context);
  
  if (menu_focused) {
    draw_menu(context);
  }

#define MAX_VERTEX_MEMORY 512 * 1024
#define MAX_ELEMENT_MEMORY 128 * 1024

  nk_sdl_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_MEMORY, MAX_ELEMENT_MEMORY);

  SDL_GL_MakeCurrent(window, original_context);

  nk_input_begin(context);
  swap_window_original(window);
  nk_input_end(context);
}
