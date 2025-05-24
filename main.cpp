#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <dlfcn.h>
#include <sys/mman.h>
#include <cstring>
#include <wchar.h>

#include "engine.hpp"
#include "entity_list.hpp"
#include "player.hpp"
#include "debug_overlay.hpp"
#include "surface.hpp"

#include "math.hpp"

#include "create_move.cpp"
#include "print.cpp"

void* get_interface(const char* lib_path, const char* version) {
  void* lib_handle = dlopen(lib_path, RTLD_NOLOAD | RTLD_NOW);
  if (!lib_handle) {
    print("Failed to load %s\n", lib_path);
    return NULL;
  }

  print("%s loaded at %p\n", lib_path, lib_handle);
  
  typedef void* (*CreateInterface)(const char*, int*);

  CreateInterface create_interface = (CreateInterface)dlsym(lib_handle, "CreateInterface");
  dlclose(lib_handle);
  
  if (!create_interface) {
    print("Failed to get CreateInterface\n");
    return NULL;
  }

  print("%s factory found at %p\n", lib_path, create_interface);

  void* interface =  create_interface(version, NULL);
    
  if (!interface) {
    print("Failed to get %s interface\n", version);
    return NULL;
  }

  print("%s interface found at %p\n", version, interface);
    
  return interface;
}

bool write_to_table(void** vtable, int index, void* func) {

  const long page_size = sysconf(_SC_PAGESIZE);

  // Sets last three digits to zero
  void* table_page = (void*)((__uint64_t)vtable & ~(page_size - 1));
  print("vfunc table page found at %p\n", table_page);

  if (mprotect(table_page, page_size, PROT_READ | PROT_WRITE) != 0) {
    print("mprotect failed to change page protection\n");
    return false;
  }

  vtable[index] = func;

  if (mprotect(table_page, page_size, PROT_READ) != 0) {
    print("mprotect failed to reset page protection\n");
    return false;
  }

  return true;
}

void (*paint_traverse_original)(void*, void*, __int8_t, __int8_t) = NULL;

void* vgui;
const char* get_panel_name(void* panel) {
    void **vtable = *(void ***)vgui;

    const char* (*get_panel_name_fn)(void*, void*) = (const char* (*)(void*, void*))vtable[37];

    return get_panel_name_fn(vgui, panel);
}


void paint_traverse_hook(void* me, void* panel, __int8_t force_repaint, __int8_t allow_force) {
  const char* panel_name = get_panel_name(panel);

  paint_traverse_original(me, panel, force_repaint, allow_force);


  if (strcmp(panel_name, "FocusOverlayPanel") != 0) {
    return;
  }

  if (!engine->is_in_game()) {
    return;
  }

  
  Player* localplayer = entity_list->player_from_index(engine->get_localplayer_index());

  surface->draw_set_text_color(255, 255, 255, 255);
  surface->draw_set_text_pos(5, 17);
  surface->draw_print_text(L"Hi Hello :3", wcslen(L"Hi Hello :3"));
  
  for (unsigned int i = 1; i <= entity_list->get_max_entities(); ++i) {
    Player* player = entity_list->player_from_index(i);

    if (player == NULL || player == localplayer) {
      continue;
    }

    
    if (player->is_dormant() || player->get_lifestate() != 1 || player->get_team() == localplayer->get_team()) {
      continue;
    }

    Vec3 location = player->get_origin();
    Vec3 screen;
    if (!overlay->world_to_screen(&location, &screen)) continue;

    float distance = distance_3d(localplayer->get_origin(), player->get_origin());
    
    surface->set_rgba(255, 255, 255, 255);

    Vec3 z_offset = {location.x, location.y, location.z + 82};
    Vec3 screen_offset;
    overlay->world_to_screen(&z_offset, &screen_offset);

    /* bone draw ID debug
    for (unsigned int h = 0; h < 128; ++h) {
      Vec3 bone = player->get_bone_pos(h);
      Vec3 bone_screen;
      overlay->world_to_screen(&bone, &bone_screen);
      surface->draw_set_text_pos(bone_screen.x, bone_screen.y);
      std::wstring a = std::to_wstring(h);
      surface->draw_print_text(a.c_str(), wcslen(a.c_str()));
    }
    */
    
    float box_offset = (13000/distance) * (float(localplayer->get_default_fov())/float(localplayer->get_fov()));
    
    //right side
    surface->draw_line(screen.x + box_offset, screen.y, screen.x + box_offset, screen_offset.y);

    //left side
    surface->draw_line(screen.x - box_offset, screen.y, screen.x - box_offset, screen_offset.y);

    //top
    surface->draw_line(screen.x - box_offset, screen_offset.y, screen.x + box_offset, screen_offset.y);

    //bottom
    surface->draw_line(screen.x - box_offset, screen.y, screen.x + box_offset, screen.y);    
    
  }
}

void** client_mode_vtable;
void** vgui_vtable;
__attribute__((constructor))
void entry() {
  usleep(500000);
  print("Hello World!\n");
  usleep(500000);
  
  void* client = get_interface("tf/bin/linux64/client.so", "VClient017");
  engine = (Engine*)get_interface("./bin/linux64/engine.so", "VEngineClient014");
  
  Vec2 resolution = engine->get_screen_size();  
  print("Resolution: %dx%d\n", resolution.x, resolution.y);

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

  
  unsigned long esp_font = surface->text_create_font();
  surface->text_set_font_glyph_set(esp_font, "ProggySquare", 14, 400, 0, 0, 0x0);
  
  /*
  void *lib_sdl_handle = dlopen("/usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0", RTLD_LAZY | RTLD_NOLOAD);

  if (!lib_sdl_handle) {
    print("Failed to load SDL2\n");
    return;
  }

  print("SDL2 loaded at %p\n", lib_sdl_handle);

  if (!init_sdl_hook(lib_sdl_handle, "SDL_PollEvent", &poll_event_hook, (void **)&poll_event_original)) {
    print("Failed to hook SDL_PollEvent\n");
    return;
  }

  if (!init_sdl_hook(lib_sdl_handle, "SDL_GL_SwapWindow", &swap_window_hook, (void **)&swap_window_original)) {
    print("Failed to hook SDL_GL_SwapWindow\n");
    return;
  }

  dlclose(lib_sdl_handle);
  */
  
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
}

