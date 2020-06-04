#ifndef AEROLITS_INCLUDE_COLLISIONS_H_
#define AEROLITS_INCLUDE_COLLISIONS_H_

#include <cmath>

namespace ktp {

static bool checkCirclesCollision(float a_radius, float a_x, float a_y, float b_radius, float b_x, float b_y);
static double distanceSquared(float x1, float y1, float x2, float y2);
template <typename T> static bool isPointInsideCircle(float cx, float cy, float radius, float x, float y);

/* DEFINITIONS BELOW */

bool checkCirclesCollision(float a_radius, float a_x, float a_y, float b_radius, float b_x, float b_y) {
  auto radii_sqrt = a_radius + b_radius;
  radii_sqrt *= radii_sqrt;
  if (distanceSquared(a_x, a_y, b_x, b_y) < radii_sqrt) return true;
  return false;
}

/* From Lazyfoo:
 * Here is the distance squared function. It's just a distance 
 * calculation ( squareRoot( x^2 + y^2 ) ) without the square root.
*/
double distanceSquared(float x1, float y1, float x2, float y2){
  auto x = x2 - x1;
  auto y = y2 - y1;
  return x * x + y * y;
}

/* From olc */
template <typename T>
bool isPointInsideCircle(T cx, T cy, T radius, T x, T y) {
  return std::sqrt((x-cx)*(x-cx) + (y-cy)*(y-cy)) < radius;
}

} // namespace ktp
#endif // AEROLITS_INCLUDE_COLLISIONS_H_