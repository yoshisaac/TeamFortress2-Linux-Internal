#ifndef MATH_HPP
#define MATH_HPP

#include <cmath>

#include "vec.hpp"

inline static float distance_3d(Vec3 location_one, Vec3 location_two) {
  return sqrt(((location_one.y - location_two.y)*(location_one.y - location_two.y)) +
	      ((location_one.x - location_two.x)*(location_one.x - location_two.x)) +
	      ((location_one.z - location_two.z)*(location_one.z - location_two.z)));
}

#endif
