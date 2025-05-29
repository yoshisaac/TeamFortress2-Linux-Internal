
#include "config.hpp"

void draw_aim_tab(struct nk_context* ctx) {
  NK_CHECKBOX_ROW(ctx, "Master", &config.aimbot.master);

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
