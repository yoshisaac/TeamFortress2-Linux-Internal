
#include "config.hpp"

void draw_aim_tab(struct nk_context* ctx) {
  NK_HEADER_ROW(ctx, "Main", NK_TEXT_LEFT); {
    NK_CHECKBOX_ROW(ctx, "Master", &config.aimbot.master);
  }  
}

void draw_esp_tab(struct nk_context* ctx) {
  NK_HEADER_ROW(ctx, "ESP", NK_TEXT_LEFT); {
    NK_CHECKBOX_ROW(ctx, "Master", &config.esp.master);
  }  
}

void draw_misc_tab(struct nk_context* ctx) {
  NK_HEADER_ROW(ctx, "misc", NK_TEXT_LEFT); {
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

    nk_layout_row_dynamic(ctx, 20, 3);
    draw_tab(ctx, "Aim", &tab, 0);
    draw_tab(ctx, "ESP", &tab, 1);
    draw_tab(ctx, "Misc", &tab, 2);

    switch (tab) {
    case 0:
      draw_aim_tab(ctx);
      break;
    case 1:
      draw_esp_tab(ctx);
      break;
    case 2:
      draw_misc_tab(ctx);
      break;
    }
  }
  nk_end(ctx);
}
