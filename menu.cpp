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

void draw_aim_tab(struct nk_context* ctx) {
  NK_HEADER_ROW(ctx, "aim", NK_TEXT_LEFT); {
  }    
}

void draw_esp_tab(struct nk_context* ctx) {
  NK_HEADER_ROW(ctx, "ESP", NK_TEXT_LEFT); {
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
  if (nk_begin(ctx, "Team Fortress 2 GNU/Linux", nk_rect(200, 200, 600, 500), NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_TITLE)) {
    static int tab = 0;

    nk_layout_row_dynamic(ctx, 30, 3);
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
