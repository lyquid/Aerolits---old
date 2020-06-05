#ifndef AEROLITS_INCLUDE_COLLISIONS_H_
#define AEROLITS_INCLUDE_COLLISIONS_H_

#include <cmath>

namespace ktp {

template <typename T> static bool checkCirclesCollision(T a_radius, T a_x, T a_y, T b_radius, T b_x, T b_y);
template <typename T> static bool checkCirclesCollisionSQRT(T a_radius, T a_x, T a_y, T b_radius, T b_x, T b_y);
template <typename T> static double distanceSquared(T x1, T y1, T x2, T y2);
template <typename T> static bool isPointInsideCircle(T circle_x, T circle_y, T radius, T point_x, T point_y);

/* DEFINITIONS BELOW */

template <typename T>
bool checkCirclesCollision(T a_radius, T a_x, T a_y, T b_radius, T b_x, T b_y) {
  auto radii_sqrt = a_radius + b_radius;
  radii_sqrt *= radii_sqrt;
  if (distanceSquared(a_x, a_y, b_x, b_y) < radii_sqrt) return true;
  return false;
}

template <typename T>
bool checkCirclesCollisionSQRT(T a_radius, T a_x, T a_y, T b_radius, T b_x, T b_y) {
  return isPointInsideCircle(a_x, a_y, a_radius + b_radius, b_x, b_y);
}

/* From LazyFoo:
 * Here is the distance squared function. It's just a distance 
 * calculation ( squareRoot( x^2 + y^2 ) ) without the square root.
*/
template <typename T>
double distanceSquared(T x1, T y1, T x2, T y2) {
  double x = static_cast<double>(x2) - static_cast<double>(x1);
  double y = static_cast<double>(y2) - static_cast<double>(y1);
  return x * x + y * y;
}

template <typename T>
bool isPointInsideCircle(T circle_x, T circle_y, T radius, T point_x, T point_y) {
  return std::sqrt((point_x - circle_x)*(point_x - circle_x) + (point_y - circle_y)*(point_y - circle_y)) < radius;
}

} // namespace ktp
#endif // AEROLITS_INCLUDE_COLLISIONS_H_