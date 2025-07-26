#include <SDL2/SDL.h>
#include <GL/glew.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_sdl2.h"

#include "config.hpp"
#include "surface.hpp"

#include "print.hpp"

#include "menu.cpp"

void (*swap_window_original)(void*) = NULL;
bool (*poll_event_original)(SDL_Event*) = NULL;

bool poll_event_hook(SDL_Event *event) {
  bool ret = poll_event_original(event);
  //nk_sdl_handle_event(event);
  if (ret)
    ImGui_ImplSDL2_ProcessEvent(event);

  
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

/*
void watermark(nk_context* ctx) {
  if (nk_begin(ctx, "watermark", nk_rect(10, 10, 150, 30), NK_WINDOW_BORDER | NK_WINDOW_NO_INPUT | NK_WINDOW_NO_SCROLLBAR)) {
    NK_TEXT_ROW(ctx, "I Use Arch BTW!!!", NK_TEXT_CENTERED);
  }

  nk_end(ctx);
}
*/

void swap_window_hook(SDL_Window* window) {
  //static struct nk_context* context = NULL;
  static SDL_GLContext original_context = NULL, new_context = NULL;
  
  if (!new_context) {
    original_context = SDL_GL_GetCurrentContext();
    new_context = SDL_GL_CreateContext(window);

    GLenum err = glewInit();
    if (err != GLEW_OK) {
      print("Failed to initialize GLEW: %s\n", glewGetErrorString(err));
      swap_window_original(window);
      return;
    }
    

    /*
      context = nk_sdl_init(window);

    set_style(context);

    struct nk_font_atlas* atlas = NULL;
    nk_sdl_font_stash_begin(&atlas);
    nk_sdl_font_stash_end();
    */
    
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplOpenGL3_Init("#version 100");
    ImGui_ImplSDL2_InitForOpenGL(window, nullptr);

    ImGuiIO& io = ImGui::GetIO();
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.WantCaptureMouse = true;
    
  }

  
  SDL_GL_MakeCurrent(window, new_context);

  if (ImGui::IsKeyPressed(ImGuiKey_Insert, false)) {
    menu_focused = !menu_focused;
  }
  
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL2_NewFrame();
  ImGui::NewFrame();

  if (menu_focused) {
    ImGui::ShowDemoWindow();

    static  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    {
      static float f = 0.0f;
      static int counter = 0;

      ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

      ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

      ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
      ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

      if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
	counter++;
      ImGui::SameLine();
      ImGui::Text("counter = %d", counter);

      ImGui::End();
    }
  }

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  /*
  get_input(context);
  watermark(context);
  
  if (menu_focused) {
    draw_menu(context);
  }
  */
#define MAX_VERTEX_MEMORY 512 * 1024
#define MAX_ELEMENT_MEMORY 128 * 1024

  //nk_sdl_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_MEMORY, MAX_ELEMENT_MEMORY);

  SDL_GL_MakeCurrent(window, original_context);

  //nk_input_begin(context);
  swap_window_original(window);
  //nk_input_end(context);
}
