#ifndef MATERIAL_SYSTEM_HPP
#define MATERIAL_SYSTEM_HPP

#include "material.hpp"

class MaterialSystem {
public:
  Material* find_material(char const* material_name, const char* texture_group_name, bool complain, const char* complain_prefix) {
    void** vtable = *(void ***)this;

    Material* (*find_material_fn)(void*, const char*, const char*, bool, const char*) =
      (Material* (*)(void*, const char*, const char*, bool, const char*))vtable[73];

    return find_material_fn(this, material_name, texture_group_name, complain, complain_prefix);
  }

  Material* create_material(const char* name, void* key_value) {
    void** vtable = *(void ***)this;

    Material* (*create_material_fn)(void*, const char*, void*) = (Material* (*)(void*, const char*, void*))vtable[72];

    return create_material_fn(this, name, key_value);
  }
};

inline static MaterialSystem* material_system;

#endif
