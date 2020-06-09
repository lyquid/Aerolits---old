#ifndef AEROLITS_INCLUDE_COLLISIONS_H_
#define AEROLITS_INCLUDE_COLLISIONS_H_

#include <cmath>
#include <SDL.h>

namespace ktp {

template <typename T> static bool checkCircleAABBCollision(T a_radius, T a_x, T a_y, T b_radius, T b_x, T b_y);
template <typename T> static bool checkCirclesCollision(T a_radius, T a_x, T a_y, T b_radius, T b_x, T b_y);
template <typename T> static bool checkCirclesCollisionSQRT(T a_radius, T a_x, T a_y, T b_radius, T b_x, T b_y);
template <typename T, typename U> static bool circleScreenCollision(T& circle_x, T& circle_y,  T radius, T& delta_x, T& delta_y, U screen_x, U screen_y);
template <typename T, typename U, typename V> static bool circleScreenCollision(T& circle, U radius, T& delta, const V& screen_size);
template <typename T> static double distanceSquared(T x1, T y1, T x2, T y2);
template <typename T> static bool isPointInsideCircleSQRT(T circle_x, T circle_y, T radius, T point_x, T point_y);
template <typename T, typename U> static void wrapCoordinates(T& point_x, T& point_y, const U size_x, const U size_y);
template <typename T, typename U> static void wrapCoordinates(T& point, const U& screen_size);

/* DEFINITIONS BELOW */

template <typename T>
bool checkCircleAABBCollision(T a_radius, T a_x, T a_y, T b_radius, T b_x, T b_y) {
  return (a_x + a_radius + b_radius > b_x
       && a_x < b_x + a_radius + b_radius
       && a_y + a_radius + b_radius > b_y
       && a_y < b_y + a_radius + b_radius);
}

/* template <typename T>
bool checkCirclesCollision(T a_radius, T a_x, T a_y, T b_radius, T b_x, T b_y) {
  auto radii_sqrt = a_radius + b_radius;
  radii_sqrt *= radii_sqrt;
  if (distanceSquared(a_x, a_y, b_x, b_y) < radii_sqrt) return true;
  return false;
} */

template <typename T>
bool checkCirclesCollisionSQRT(T a_radius, T a_x, T a_y, T b_radius, T b_x, T b_y) {
  return isPointInsideCircleSQRT(a_x, a_y, a_radius + b_radius, b_x, b_y);
}

template <typename T, typename U>
bool circleScreenCollision(T& circle_x, T& circle_y, T radius, T& delta_x, T& delta_y, U screen_x, U screen_y) {
  bool collision = false;
  if (circle_x - radius < 0) {
    circle_x = radius;
    delta_x = -delta_x;
    collision = true;
  } else if (circle_x + radius >= screen_x) {
    circle_x = screen_x - radius;
    delta_x = -delta_x;
    collision = true;
  }
  if (circle_y - radius < 0) {
    circle_y = radius;
    delta_y = -delta_y;
    collision = true;
  } else if (circle_y + radius >= screen_y) {
    circle_y = screen_y - radius;
    delta_y = -delta_y;
    collision = true;
  }
  return collision;
}

template <typename T, typename U, typename V> 
bool circleScreenCollision(T& circle, U radius, T& delta, const V& screen_size) {
  return circleScreenCollision(circle.x, circle.y, radius, delta.x, delta.y, screen_size.x, screen_size.y);
}

/* From LazyFoo:
 * Here is the distance squared function. It's just a distance 
 * calculation ( squareRoot( x^2 + y^2 ) ) without the square root.
*/
/* template <typename T>
double distanceSquared(T x1, T y1, T x2, T y2) {
  double x = static_cast<double>(x2) - static_cast<double>(x1);
  double y = static_cast<double>(y2) - static_cast<double>(y1);
  return x * x + y * y;
} */

template <typename T>
bool isPointInsideCircleSQRT(T circle_x, T circle_y, T radius, T point_x, T point_y) {
  return std::sqrt((point_x - circle_x)*(point_x - circle_x) + (point_y - circle_y)*(point_y - circle_y)) < radius;
}

template <typename T, typename U>
void wrapCoordinates(T& point_x, T& point_y, const U size_x, const U size_y) {
  if (point_x < 0) {
    point_x = size_x + point_x;
  } else if (point_x > size_x - 1) {
    point_x = point_x - size_x;
  }

  if (point_y < 0) {
    point_y = size_y + point_y;
  } else if (point_y > size_y - 1) {
    point_y = point_y - size_y;
  }
}

template <typename T, typename U>
void wrapCoordinates(T& point, const U& screen_size) {
  wrapCoordinates(point.x, point.y, screen_size.x, screen_size.y);
}

} // namespace ktp
#endif // AEROLITS_INCLUDE_COLLISIONS_H_