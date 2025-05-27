#include "print.hpp"

#include <unistd.h>

#include "engine.hpp"
#include "entity_list.hpp"
#include "debug_overlay.hpp"
#include "surface.hpp"

#include "hooks.cpp"

#include "sdl.cpp"
#include "create_move.cpp"
#include "paint_traverse.cpp"


void** client_mode_vtable;
void** vgui_vtable;
__attribute__((constructor))
void entry() {
  usleep(50000);
  
  void* client = get_interface("tf/bin/linux64/client.so", "VClient017");
  engine = (Engine*)get_interface("./bin/linux64/engine.so", "VEngineClient014");
  
  vgui = get_interface("./bin/linux64/vgui2.so", "VGUI_Panel009");
  surface = (Surface*)get_interface("./bin/linux64/vguimatsurface.so", "VGUI_Surface030");

  overlay = (DebugOverlay*)get_interface("./bin/linux64/engine.so", "VDebugOverlay003");
  
  entity_list = (EntityList*)get_interface("./tf/bin/linux64/client.so", "VClientEntityList003");
  
  void** client_vtable = *(void ***)client;
  void *hud_process_input_addr = client_vtable[10];
  __uint32_t client_mode_eaddr = *(__uint32_t *)((__uint64_t)(hud_process_input_addr) + 0x3);
  void *client_mode_next_instruction = (void *)((__uint64_t)(hud_process_input_addr) + 0x7);
  void *client_mode_interface = *(void **)((__uint64_t)(client_mode_next_instruction) + client_mode_eaddr);

  client_mode_vtable = *(void***)client_mode_interface;
  
  create_move_original = (bool (*)(void*, float, void*))client_mode_vtable[22];

  if (!write_to_table(client_mode_vtable, 22, (void*)create_move_hook)) {
    print("CreateMove hook failed\n");
  } else {
    print("Holy moly we hooking!\n");
  }

  vgui_vtable = *(void***)vgui;

  paint_traverse_original = (void (*)(void*, void*, __int8_t, __int8_t))vgui_vtable[42];
  

  if (!write_to_table(vgui_vtable, 42, (void*)paint_traverse_hook)) {
    print("PaintTraverse hook failed\n");
  } else {
    print("Holy moly we hooking! x2\n");
  }
  
  void* lib_sdl_handle = dlopen("/usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0", RTLD_LAZY | RTLD_NOLOAD);

  if (!lib_sdl_handle) {
    print("Failed to load SDL2\n");
    return;
  }

  print("SDL2 loaded at %p\n", lib_sdl_handle);

  if (!sdl_hook(lib_sdl_handle, "SDL_PollEvent", (void*)poll_event_hook, (void **)&poll_event_original)) {
    print("Failed to hook SDL_PollEvent\n");
    return;
  }

  if (!sdl_hook(lib_sdl_handle, "SDL_GL_SwapWindow", (void*)swap_window_hook, (void **)&swap_window_original)) {
    print("Failed to hook SDL_GL_SwapWindow\n");
    return;
  }

  dlclose(lib_sdl_handle);
  
  void *hud_update = client_vtable[11];
  __uint32_t global_vars_eaddr = *(__uint32_t *)((__uint64_t)(hud_update) + 0x16);
  void *global_vars_next_instruction = (void *)((__uint64_t)(hud_update) + 0x1A);
  //set_global_vars_ptr(*(void **)((__uint64_t)(global_vars_next_instruction) + global_vars_eaddr));
  
  
  return;
}

__attribute__((destructor))
void exit() {
  if (!write_to_table(client_mode_vtable, 22, (void*)create_move_original)) {
    print("CreateMove failed to restore hook\n");
  }

  if (!write_to_table(vgui_vtable, 42, (void*)paint_traverse_original)) {
    print("PaintTraverse failed to restore hook\n");
  }   

  void *lib_sdl_handle = dlopen("/usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0", RTLD_LAZY | RTLD_NOLOAD);

  if (!restore_sdl_hook(lib_sdl_handle, "SDL_GL_SwapWindow", (void*)swap_window_original)) {
    print("Failed to restore SDL_GL_SwapWindow\n");
  }
    
  if (!restore_sdl_hook(lib_sdl_handle, "SDL_PollEvent", (void*)poll_event_original)) {
    print("Failed to restore SDL_PollEvent\n");
  }

  dlclose(lib_sdl_handle);

  if (menu_focused == true)
    surface->set_cursor_visible(!menu_focused);
}

