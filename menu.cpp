#include "config.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "print.hpp"

#include "surface.hpp"

#include "imgui/dearimgui.hpp"

#include <SDL2/SDL_mouse.h>

inline static bool menu_focused = false;

static ImGuiStyle orig_style;

void get_input(SDL_Event* event) {
  ImGui::KeybindEvent(event, &config.aimbot.key.waiting, &config.aimbot.key.button);
  ImGui::KeybindEvent(event, &config.visuals.thirdperson_key.waiting, &config.visuals.thirdperson_key.button);
}

void draw_aim_tab() {
  ImGui::NewLine();
  ImGui::Checkbox("Master", &config.aimbot.master);


  ImGui::EndGroup();

  ImGui::SameLine();
  ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
  ImGui::SameLine();


  ImGui::BeginGroup();

  ImGui::Checkbox("Auto Shoot", &config.aimbot.auto_shoot);  

  ImGui::Text("Aimbot botton: ");
  ImGui::SameLine();
  ImGui::KeybindBox(&config.aimbot.key.waiting, &config.aimbot.key.button);
  ImGui::SameLine();
  ImGui::Checkbox("Use Button", &config.aimbot.use_key);

  ImGui::Checkbox("Silent", &config.aimbot.silent);

  
  ImGui::Text("FOV: ");
  ImGui::SameLine();
  ImGui::SliderFloat(" ", &config.aimbot.fov, 0.1f, 180.0f, "%.0f\xC2\xB0");
  
  ImGui::Checkbox("Draw FOV", &config.aimbot.draw_fov);
  
  ImGui::EndGroup();  
}

void draw_esp_tab() {  
  ImGui::NewLine();
  ImGui::Checkbox("Master", &config.esp.master);

  ImGui::EndGroup();

  ImGui::SameLine();
  ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
  ImGui::SameLine();

  /* ESP */
  ImGui::BeginGroup();  

  //Player
  ImGui::BeginGroup();
  ImGui::Text("Player");
  ImGui::Checkbox("Box##Player", &config.esp.player.box);
  ImGui::Checkbox("Health Bar##Player", &config.esp.player.health_bar);
  ImGui::Checkbox("Name##Player", &config.esp.player.name);
  ImGui::NewLine();
  ImGui::Text("Flags");
  ImGui::Checkbox("Target Indicator##Player", &config.esp.player.health_bar);
  ImGui::EndGroup();

  ImGui::SameLine();
  ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
  ImGui::SameLine();

  //Pickups (health and ammo etc)
  ImGui::BeginGroup();
  ImGui::Text("Pickup");
  ImGui::Checkbox("Box##Pickup", &config.esp.pickup.box);
  ImGui::Checkbox("Name##Pickup", &config.esp.pickup.name);
  ImGui::EndGroup();
    
  ImGui::EndGroup();
}

void draw_visuals_tab() {
  ImGui::EndGroup();

  ImGui::SameLine();
  ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
  ImGui::SameLine();

  /* Visuals */
  ImGui::BeginGroup();

  /* Removals */ //maybe make me a drop down
  ImGui::BeginGroup();
  ImGui::Text("Removals");
  ImGui::Checkbox("Scope", &config.visuals.hide_scope);
  ImGui::Checkbox("Zoom", &config.visuals.remove_zoom);

  ImGui::NewLine();
  ImGui::NewLine();
  ImGui::NewLine();
  ImGui::NewLine();
  
  ImGui::EndGroup();

  ImGui::SameLine();
  ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
  ImGui::SameLine();

  /* Camera */
  ImGui::BeginGroup();
  ImGui::Text("Camera");
  ImGui::Text("Key: "); ImGui::SameLine();
  ImGui::KeybindBox(&config.visuals.thirdperson_key.waiting, &config.visuals.thirdperson_key.button); ImGui::SameLine();
  ImGui::Checkbox("Thirdperson", &config.visuals.thirdperson);  

  
  ImGui::NewLine();
  ImGui::NewLine();
  
  ImGui::Checkbox("Override FOV", &config.visuals.override_fov);
  ImGui::SliderFloat(" ", &config.visuals.custom_fov, 30.1f, 150.0f, "%.0f\xC2\xB0");
  ImGui::EndGroup();
  
  ImGui::EndGroup();
}

void draw_misc_tab() {
  ImGui::EndGroup();

  ImGui::SameLine();
  ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
  ImGui::SameLine();

  ImGui::BeginGroup();
  
  ImGui::Text("General");
  ImGui::Checkbox("Bhop", &config.misc.bhop);
  ImGui::Checkbox("Bypass sv_pure", &config.misc.bypasspure);
  ImGui::Checkbox("No Push", &config.misc.no_push);  

  ImGui::EndGroup();
}


void draw_tab(ImGuiStyle* style, const char* name, int* tab, int index) {
  ImVec4 orig_box_color = ImVec4(0.15, 0.15, 0.15, 1);
  
  if (*tab == index) {
    style->Colors[ImGuiCol_Button] = ImVec4(orig_box_color.x + 0.15, orig_box_color.y + 0.15, orig_box_color.z + 0.15, 1.00f);
  } else {
    style->Colors[ImGuiCol_Button] = ImVec4(0.15, 0.15, 0.15, 1);
  }
  
  if (ImGui::Button(name, ImVec2(80, 30))) {
    *tab = index;
  }
  style->Colors[ImGuiCol_Button] = ImVec4(0.15, 0.15, 0.15, 1);
}

void draw_menu() {
  ImGui::SetNextWindowSize(ImVec2(600, 350));
  if (ImGui::Begin("Team Fortress 2 GNU/Linux", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse)) {
    static int tab = 0;

    ImGuiStyle* style = &ImGui::GetStyle();

    style->Colors[ImGuiCol_WindowBg]         = ImVec4(0.1, 0.1, 0.1, 1);
    style->Colors[ImGuiCol_TitleBgActive]    = ImVec4(0.05, 0.05, 0.05, 1);
    style->Colors[ImGuiCol_TitleBg]          = ImVec4(0.05, 0.05, 0.05, 1);
    style->Colors[ImGuiCol_CheckMark]        = ImVec4(0.869346734, 0.450980392, 0.211764706, 1);
    style->Colors[ImGuiCol_FrameBg]          = ImVec4(0.15, 0.15, 0.15, 1);
    style->Colors[ImGuiCol_FrameBgHovered]   = ImVec4(0.869346734, 0.450980392, 0.211764706, 0.5);
    style->Colors[ImGuiCol_FrameBgActive]    = ImVec4(0.919346734, 0.500980392, 0.261764706, 0.6);
    style->Colors[ImGuiCol_ButtonHovered]    = ImVec4(0.869346734, 0.450980392, 0.211764706, 0.5);
    style->Colors[ImGuiCol_ButtonActive]     = ImVec4(0.919346734, 0.500980392, 0.261764706, 0.6);
    style->Colors[ImGuiCol_SliderGrab]       = ImVec4(0.869346734, 0.450980392, 0.211764706, 1);
    style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.899346734, 0.480980392, 0.241764706, 1);
    
    
    ImGui::BeginGroup();
    draw_tab(style, "Aimbot", &tab, 0);
    draw_tab(style, "ESP", &tab, 1);
    draw_tab(style, "Visuals", &tab, 2);
    draw_tab(style, "Misc", &tab, 3);

    switch (tab) {
    case 0:
      draw_aim_tab();
      break;
    case 1:
      draw_esp_tab();
      break;      
    case 2:
      draw_visuals_tab();
      break;
    case 3:
      draw_misc_tab();
      break;
    }
  }
  
  ImGui::End();  
}

