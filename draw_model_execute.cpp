#include <string.h>

#include "vec.hpp"

#include "entity_list.hpp"

#include "material_system.hpp"
#include "material.hpp"

#include "model_render.hpp"
#include "render_view.hpp"

#include "chams.cpp"

void draw_model_execute_hook(void* me, void* state, ModelRenderInfo_t* pinfo, VMatrix* bone_to_world) {

  static RGBA_float original_color;
  render_view->get_color_modulation(&original_color);
  original_color.a = render_view->get_blend();

  chams(me, state, pinfo, bone_to_world);
  
  draw_model_execute_original(me, state, pinfo, bone_to_world);

  render_view->set_color_modulation(&original_color);
  render_view->set_blend(original_color.a);

  model_render->forced_material_override(NULL, 0);
}
