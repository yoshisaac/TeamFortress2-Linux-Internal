#include "print.hpp"

#include <unistd.h>

#include "player.hpp"

#include "engine.hpp"
#include "client.hpp"
#include "entity_list.hpp"
#include "debug_overlay.hpp"
#include "surface.hpp"
#include "engine_trace.hpp"
#include "render_view.hpp"
#include "material_system.hpp"
#include "model_render.hpp"
#include "convar_system.hpp"

#include "hooks.cpp"
#include "memory.hpp"
#include "libsigscan/libsigscan.h"
#include "funchook/funchook.h"


#include "sdl.cpp"
//#include "vulkan.cpp"

#include "create_move.cpp"
#include "paint_traverse.cpp"
#include "override_view.cpp"
#include "draw_view_model.cpp"
#include "in_cond.cpp"
#include "draw_model_execute.cpp"
#include "load_white_list.cpp"

#include "vec.hpp"

void** client_mode_vtable;
void** vgui_vtable;
void** model_render_vtable;


funchook_t* funchook;

__attribute__((constructor))
void entry() {
  usleep(50000);

  funchook = funchook_create();
  
  client = (Client*)get_interface("./tf/bin/linux64/client.so", "VClient017");
  engine = (Engine*)get_interface("./bin/linux64/engine.so", "VEngineClient014");
  
  vgui = get_interface("./bin/linux64/vgui2.so", "VGUI_Panel009");
  surface = (Surface*)get_interface("./bin/linux64/vguimatsurface.so", "VGUI_Surface030");

  overlay = (DebugOverlay*)get_interface("./bin/linux64/engine.so", "VDebugOverlay003");
  
  entity_list = (EntityList*)get_interface("./tf/bin/linux64/client.so", "VClientEntityList003");

  render_view = (RenderView*)get_interface("./bin/linux64/engine.so", "VEngineRenderView014");

  engine_trace = (EngineTrace*)get_interface("./bin/linux64/engine.so", "EngineTraceClient003");

  model_render = (ModelRender*)get_interface("./bin/linux64/engine.so", "VEngineModel016");

  material_system = (MaterialSystem*)get_interface("./bin/linux64/materialsystem.so", "VMaterialSystem082");

  convar_system = (ConvarSystem*)get_interface("./bin/linux64/libvstdlib.so", "VEngineCvar004");
  
  void** client_vtable = *(void ***)client;
  void* hud_process_input_addr = client_vtable[10];
  __uint32_t client_mode_eaddr = *(__uint32_t *)((__uint64_t)(hud_process_input_addr) + 0x3);
  void* client_mode_next_instruction = (void *)((__uint64_t)(hud_process_input_addr) + 0x7);
  void* client_mode_interface = *(void **)((__uint64_t)(client_mode_next_instruction) + client_mode_eaddr);

  client_mode_vtable = *(void***)client_mode_interface;
  
  create_move_original = (bool (*)(void*, float, user_cmd*))client_mode_vtable[22];
  if (!write_to_table(client_mode_vtable, 22, (void*)create_move_hook)) {
    print("CreateMove hook failed\n");
  } else {
    print("CreateMove hooked\n");
  }

  override_view_original = (void (*)(void*, view_setup*))client_mode_vtable[17];  
  if (!write_to_table(client_mode_vtable, 17, (void*)override_view_hook)) {
    print("OverrideView hook failed\n");
  } else {
    print("OverrideView hooked\n");
  }

  draw_view_model_original = (bool (*)(void*))client_mode_vtable[25];  
  if (!write_to_table(client_mode_vtable, 25, (void*)draw_view_model_hook)) {
    print("ShouldDrawViewModel hook failed\n");
  } else {
    print("ShouldDrawViewModel hooked\n");
  }
  
  vgui_vtable = *(void ***)vgui;

  paint_traverse_original = (void (*)(void*, void*, __int8_t, __int8_t))vgui_vtable[42];  
  if (!write_to_table(vgui_vtable, 42, (void*)paint_traverse_hook)) {
    print("PaintTraverse hook failed\n");
  } else {
    print("PaintTraverse hooked\n");
  }

  model_render_vtable = *(void ***)model_render;  
  
  draw_model_execute_original = (void (*)(void*, void*, ModelRenderInfo_t*, VMatrix*))model_render_vtable[19];  
  if (!write_to_table(model_render_vtable, 19, (void*)draw_model_execute_hook)) {
    print("DrawModelExecute hook failed\n");
  } else {
    print("DrawModelExecute hooked\n");
  }

  /* //I'm going crazy trying to get the Local Player to render while scoped.
     // pls help. All i want is to hook "C_TFPlayer::ShouldDraw()"
     // - Dr_Coomer

     //Hooking in_cond has been a partial fix to the issue. Hooking
     // ShouldDraw would maybe be better, but this works good for now. :3
   */

  void* client_base_address = get_module_base_address("client.so");
  void* engine_base_address = get_module_base_address("engine.so");

  //in_cond_original = (bool (*)(void*, int))((unsigned long)client_base_address + 0x1CD4920); //raw dog the base address (this is fucking stupid, use a signature instead)

  in_cond_original = (bool (*)(void*, int))sigscan_module("client\\.so", "55 83 FE ? 48 89 E5 41 54 41 89 F4");  
  //in_cond_original = (bool (*)(void*, int))((unsigned long)client_base_address + 0x1CD4920); //raw dog the base address (this is fucking stupid, use a signature instead)

  load_white_list_original = (void* (*)(void*, const char*))((unsigned long)engine_base_address + 0x3B3880);

  int rv;
  
  rv = funchook_prepare(funchook, (void**)&in_cond_original, (void*)in_cond_hook);
  if (rv != 0) {
  }

  rv = funchook_prepare(funchook, (void**)&load_white_list_original, (void*)load_white_list_hook);
  if (rv != 0) {
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

  /*
  void* lib_vulkan_handle = dlopen("/run/host/usr/lib/libvulkan.so.1.4.313", RTLD_LAZY | RTLD_NOLOAD);
  
  if (!lib_vulkan_handle) {
    print("Failed to load vulkan\n");
    return;
  }

  queue_present_original = (VkResult (*)(VkQueue, const VkPresentInfoKHR*))dlsym(lib_vulkan_handle, "vkQueuePresentKHR");

  if (!queue_present_original) {
    print("Failed to locate vkQueuePresentKHR\n");
  } else {
    print("vkQueuePresentKHR located at %p\n", queue_present_original);
  }

  rv = funchook_prepare(funchook, (void**)&queue_present_original, (void*)queue_present_hook);
  if (rv != 0) {
  }  
  */
  
  //dlclose(lib_vulkan_handle);

  
  void* hud_update = client_vtable[11];
  __uint32_t global_vars_eaddr = *(__uint32_t *)((__uint64_t)(hud_update) + 0x16);
  void* global_vars_next_instruction = (void *)((__uint64_t)(hud_update) + 0x1A);
  //set_global_vars_ptr(*(void **)((__uint64_t)(global_vars_next_instruction) + global_vars_eaddr));
  
  
  return;
}

__attribute__((destructor))
void exit() {
  if (!write_to_table(client_mode_vtable, 22, (void*)create_move_original)) {
    print("CreateMove failed to restore hook\n");
  }

  if (!write_to_table(client_mode_vtable, 17, (void*)override_view_original)) {
    print("OverrideView failed to restore hook\n");
  }

  if (!write_to_table(client_mode_vtable, 25, (void*)draw_view_model_original)) {
    print("ShouldDrawViewModel failed to restore hook\n");
  }
  
  if (!write_to_table(vgui_vtable, 42, (void*)paint_traverse_original)) {
    print("PaintTraverse failed to restore hook\n");
  }   

  if (!write_to_table(model_render_vtable, 19, (void*)draw_model_execute_original)) {
    print("DrawModelExecute failed to restore hook\n");
  }

  
  funchook_uninstall(funchook, 0);

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

