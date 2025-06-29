#include "config.hpp"

#include "print.hpp"

#include "surface.hpp"

#include <SDL2/SDL_mouse.h>

inline static bool menu_focused = false;

void set_style(nk_context* ctx) {
  struct nk_color table[NK_COLOR_COUNT];
  memcpy(table, nk_default_color_style, sizeof(nk_default_color_style));

  table[NK_COLOR_TOGGLE_CURSOR] = nk_rgba(207, 115, 54, 255);
  table[NK_COLOR_TOGGLE_HOVER] = nk_rgba(207, 115, 54, 255 / 2);

  nk_style_from_table(ctx, table);
}


void get_input(nk_context* ctx) {
  if (nk_input_is_key_pressed(&ctx->input, NK_KEY_DEL)) {
    menu_focused = !menu_focused;
    surface->set_cursor_visible(menu_focused);
  }
}


void draw_aim_tab(struct nk_context* ctx) {
  NK_CHECKBOX_ROW(ctx, "Master", &config.aimbot.master);

  NK_CHECKBOX_ROW(ctx, "Auto Shoot", &config.aimbot.auto_shoot);
  
  nk_layout_row_static(ctx, 20, 100, 5);
  nk_label(ctx, "Aimbot Button: ", NK_TEXT_LEFT);
  char button_edit_buffer[64];
  if (config.aimbot.key.button_type == INPUT_KEY) {
    sprintf(button_edit_buffer, SDL_GetScancodeName((SDL_Scancode)config.aimbot.key.button));
  } else if (config.aimbot.key.button_type == INPUT_MOUSE) {
    switch (config.aimbot.key.button) {
    case SDL_BUTTON_MIDDLE: 
      sprintf(button_edit_buffer, "Middle Mouse"); break;
    case SDL_BUTTON_RIGHT:
      sprintf(button_edit_buffer, "Right Mouse"); break;
    case SDL_BUTTON_X1:
      sprintf(button_edit_buffer, "Side Mouse 1"); break;
    case SDL_BUTTON_X2:
      sprintf(button_edit_buffer, "Side Mouse 2"); break;}
  }    
  nk_button_set_behavior(ctx, NK_BUTTON_REPEATER);
  if (nk_button_label(ctx, button_edit_buffer)) {
    get_button_down(&config.aimbot.key);
  }
  nk_checkbox_label(ctx, "Use Button", &config.aimbot.use_key);
    
  
  NK_CHECKBOX_ROW(ctx, "Silent", &config.aimbot.silent);    

  char fov_text[32];
  sprintf(fov_text, "FOV: %.0f\xC2\xB0", config.aimbot.fov);
  NK_FLOAT_SLIDER_ROW(ctx, fov_text, &config.aimbot.fov, 1.0f, 180.0f, 1.0f); 
  NK_CHECKBOX_ROW(ctx, "Draw FOV", &config.aimbot.draw_fov);    

  NK_HEADER_ROW(ctx, "General", NK_TEXT_CENTERED); {
  }
}

void draw_esp_tab(struct nk_context* ctx) {  
  NK_CHECKBOX_ROW(ctx, "Master", &config.esp.master);
  
  NK_HEADER_ROW(ctx, "Player", NK_TEXT_CENTERED); {
    NK_CHECKBOX_ROW(ctx, "Box", &config.esp.player.box);
    NK_CHECKBOX_ROW(ctx, "Health Bar", &config.esp.player.health_bar);
    NK_CHECKBOX_ROW(ctx, "Name", &config.esp.player.name);
    NK_CHECKBOX_ROW(ctx, "Target Indicator", &config.esp.player.target_indicator);
  }
  
  NK_HEADER_ROW(ctx, "Pickup", NK_TEXT_CENTERED); {
    NK_CHECKBOX_ROW(ctx, "Box", &config.esp.pickup.box);
    NK_CHECKBOX_ROW(ctx, "Name", &config.esp.pickup.name);
  }
}

void draw_visuals_tab(struct nk_context* ctx) {
  NK_HEADER_ROW(ctx, "General", NK_TEXT_CENTERED); {
    NK_CHECKBOX_ROW(ctx, "Hide Scope", &config.visuals.hide_scope);
    NK_CHECKBOX_ROW(ctx, "Remove Zoom", &config.visuals.remove_zoom);

    nk_layout_row_static(ctx, 20, 100, 5);
    nk_label(ctx, "Key: ", NK_TEXT_LEFT);
    char button_edit_buffer[64];
    if (config.visuals.thirdperson_key.button_type == INPUT_KEY) {
      sprintf(button_edit_buffer, SDL_GetScancodeName((SDL_Scancode)config.visuals.thirdperson_key.button));
    } else if (config.visuals.thirdperson_key.button_type == INPUT_MOUSE) {
      switch (config.visuals.thirdperson_key.button) {
      case SDL_BUTTON_MIDDLE: 
	sprintf(button_edit_buffer, "Middle Mouse"); break;
      case SDL_BUTTON_RIGHT:
	sprintf(button_edit_buffer, "Right Mouse"); break;
      case SDL_BUTTON_X1:
	sprintf(button_edit_buffer, "Side Mouse 1"); break;
      case SDL_BUTTON_X2:
	sprintf(button_edit_buffer, "Side Mouse 2"); break;}
    }    
    nk_button_set_behavior(ctx, NK_BUTTON_REPEATER);
    if (nk_button_label(ctx, button_edit_buffer)) {
      get_button_down(&config.visuals.thirdperson_key);
    }
    nk_checkbox_label(ctx, "Thirdperson", &config.visuals.thirdperson);    

  }

  
  NK_HEADER_ROW(ctx, "View", NK_TEXT_CENTERED); {
    NK_CHECKBOX_ROW(ctx, "Override FOV", &config.visuals.override_fov);
    char fov_text[32];
    sprintf(fov_text, "FOV: %.0f\xC2\xB0", config.visuals.custom_fov);
    NK_FLOAT_SLIDER_ROW(ctx, fov_text, &config.visuals.custom_fov, 30.0f, 150.0f, 1.0f); 
  }
}

void draw_misc_tab(struct nk_context* ctx) {
  NK_HEADER_ROW(ctx, "General", NK_TEXT_CENTERED); {
    NK_CHECKBOX_ROW(ctx, "Bhop", &config.misc.bhop);
    NK_CHECKBOX_ROW(ctx, "Bypass sv_pure", &config.misc.bypasspure);
  }  
}


void draw_tab(struct nk_context* ctx, const char* name, int* tab, int index)
{
    if (*tab == index)
    {
        ctx->style.button.normal.data.color = nk_rgb(35, 35, 35);
    }
    else
    {
        ctx->style.button.normal.data.color = nk_rgb(50, 50, 50);
    }
    if (nk_button_label(ctx, name))
    {
        *tab = index;
    }
    ctx->style.button.normal.data.color = nk_rgb(50, 50, 50);
}


void draw_menu(struct nk_context* ctx) {
  if (nk_begin(ctx, "Team Fortress 2 GNU/Linux", nk_rect(200, 200, 600, 350), NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_TITLE)) {
    static int tab = 0;

    nk_layout_row_dynamic(ctx, 20, 4);
    draw_tab(ctx, "Aim", &tab, 0);
    draw_tab(ctx, "ESP", &tab, 1);
    draw_tab(ctx, "Visuals", &tab, 2);
    draw_tab(ctx, "Misc", &tab, 3);

    switch (tab) {
    case 0:
      draw_aim_tab(ctx);
      break;
    case 1:
      draw_esp_tab(ctx);
      break;
    case 2:
      draw_visuals_tab(ctx);
      break;
    case 3:
      draw_misc_tab(ctx);
      break;
    }
  }
  nk_end(ctx);
}
