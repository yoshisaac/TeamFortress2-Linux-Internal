#ifndef ENGINE_TRACE_HPP
#define ENGINE_TRACE_HPP

#include "vec.hpp"

enum trace_type_t {
  TRACE_EVERYTHING = 0,
  TRACE_WORLD_ONLY,
  TRACE_ENTITIES_ONLY,
  TRACE_EVERYTHING_FILTER_PROPS,
};

struct ray_t
{
  struct Vec3_aligned start;	    // starting point, centered within the extents
  struct Vec3_aligned delta;	    // direction + length of the ray
  struct Vec3_aligned start_offset;	// Add this to m_Start to get the actual ray start
  struct Vec3_aligned extents;	    // Describes an axis aligned box extruded along a ray
  bool is_ray;	                    // are the extents zero?
  bool is_swept;	                    // is delta != 0?
};

struct trace_filter {
  void** vtable;
  void* skip;
};

struct cplane_t {
  Vec3 normal;
  float dist;
  unsigned char type;
  unsigned char signbits;
  unsigned char pad[2];
};

struct csurface_t {
  const char* name;
  short surface_props;
  unsigned short flags;
};

struct trace_t {
  Vec3 startpos;
  Vec3 endpos;
  struct cplane_t plane;
  float fraction;
  int contents;
  unsigned short disp_flags;
  bool all_solid;
  bool start_solid;
  float fraction_left_solid;
  struct csurface_t surface;
  int hit_group;
  short physics_bone;
  void* entity;
  int hitbox;

  // bool did_hit() const { return m_fraction < 1.f || m_allsolid || m_start_solid; }
};

//add a namespace or class for these functions and vars
bool should_hit_entity(struct trace_filter* interface, void* entity, int contents_mask) {
  return entity != interface->skip;
}

enum trace_type_t trace_type = TRACE_EVERYTHING;
void set_type(enum trace_type_t new_trace_type) {
  trace_type = new_trace_type;
}

enum trace_type_t get_type(struct trace_filter* interface) {
  return trace_type;
}

static void* trace_filter_vtable[2] = { (void*)should_hit_entity, (void*)get_type };


class EngineTrace {
public:
  struct Vec3_aligned Vec3_aligned_subtract(Vec3* a, Vec3* b) {
    struct Vec3_aligned result = {
      .x = a->x - b->x,
      .y = a->y - b->y,
      .z = a->z - b->z
    };

    return result;
  }

  struct ray_t init_ray(Vec3* start, Vec3* end) {
    struct Vec3_aligned delta = Vec3_aligned_subtract(end, start);
    bool is_swept = (delta.x != 0.0f || delta.y != 0.0f || delta.z != 0.0f);

    struct ray_t ray = {
      .start = { start->x, start->y, start->z },
      .delta = { delta.x, delta.y, delta.z },
      .start_offset = { 0.0f, 0.0f, 0.0f },
      .extents = { 0.0f, 0.0f, 0.0f },
      .is_ray = true,
      .is_swept = is_swept
    };

    return ray;
  }

  void init_trace_filter(struct trace_filter* filter, void* skip) {
    filter->vtable = trace_filter_vtable;
    filter->skip = skip;
  }

  void trace_ray(struct ray_t* ray, unsigned int f_mask, struct trace_filter* p_trace_filter, struct trace_t* p_trace) {
    void** vtable = *(void ***)this;
    void (*trace_ray_fn)(void*, struct ray_t*, unsigned int, struct trace_filter*, struct trace_t*) =
      (void (*)(void*, struct ray_t*, unsigned int, struct trace_filter*, struct trace_t*))vtable[4];

    trace_ray_fn(this, ray, f_mask, p_trace_filter, p_trace);
  }
};

inline static EngineTrace* engine_trace;

#endif
