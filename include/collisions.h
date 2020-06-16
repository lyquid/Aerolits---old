#ifndef AEROLITS_INCLUDE_COLLISIONS_H_
#define AEROLITS_INCLUDE_COLLISIONS_H_

#include <cmath>
#include <SDL.h>

namespace ktp {

/** Performs an axis-aligned bounding box collision detection.
 *  @returns true if there is a collision.
 **/
template <typename T>             
static bool checkCircleAABBCollision(T a_x, T a_y, T a_radius, T b_x, T b_y, T b_radius);
template <typename T, typename U> 
static bool checkCircleAABBCollision(const T& circle_a, U a_radius, const T& circle_b, U b_radius);

/** Checks if 2 circles are colliding by comparing the sum of 
 *  their radius to the distance between their centers.
 *  *Warning* It uses an sqrtf() call.
 *  @returns true if there is a collision.
 **/
template <typename T> 
static bool checkCirclesCollision(T a_x, T a_y, T a_radius, T b_x, T b_y,  T b_radius);
template <typename T, typename U>
static bool checkCirclesCollision(T circle_a, U a_radius, T circle_b, U b_radius);

/** Checks if a circle is colliding with the screen border,
 *  changing its position and deltaV accordingly if there is a collision.
 *  @returns true if there is a collision.
 **/
template <typename T, typename U>             
static bool circleScreenCollision(T& circle_x, T& circle_y,  T radius, T& delta_x, T& delta_y, U screen_x, U screen_y);
template <typename T, typename U, typename V> 
static bool circleScreenCollision(T& circle, U radius, T& delta, const V& screen_size);

/** Checks if a circle is out of the screen, even for a pixel.
 *  @returns true if the circle is out of the screen.
 **/
template <typename T, typename U>             
static bool checkCircleOutScreen(T circle_x, T circle_y, T radius, U screen_x, U screen_y);
template <typename T, typename U, typename V> 
static bool checkCircleOutScreen(const T& circle, U radius, const V& screen_size);

/** Calculates the distance between 2 points.
 *  *Warning* Calls sqrtf().
 *  @returns the distance between the 2 points.
 **/
template <typename T> 
static float distanceBetweenPoints(T a_x, T a_y, T b_x, T b_y);
template <typename T> 
static float distanceBetweenPoints(const T& a, const T& b);

/** Calculates an elastic collision between 2 circles.
 *  Updates the deltaV of the circles.
 *  *Warning* Calls sqrtf() if no distance is provided.
 **/
template <typename T> 
static void elasticCollision(T a_x, T a_y, T& a_dx, T& a_dy, T a_m, T b_x, T b_y, T& b_dx, T& b_dy, T b_m, T distance);
template <typename T> 
static void elasticCollision(T a_x, T a_y, T& a_dx, T& a_dy, T a_m, T b_x, T b_y, T& b_dx, T& b_dy, T b_m);
template <typename T, typename U>
static void elasticCollision(const T& circle_a, T& a_delta, U a_mass, const T& circle_b, T&b_delta, U b_mass);
template <typename T, typename U>
static void elasticCollision(const T& circle_a, T& a_delta, U a_mass, const T& circle_b, T&b_delta, U b_mass, U distance);

/** Checks if a point is inside a circle.
 *  *Warning* Calls sqrtf().
 *  @returns ture if the point is inside the circle.
 **/
template <typename T> 
static bool isPointInsideCircle(T circle_x, T circle_y, T radius, T point_x, T point_y);
template <typename T, typename U> 
static bool isPointInsideCircle(const T& circle, U radius, const T& point);

/** Checks if a point is outside the screen an wraps its coordinates
 *  in order to make it appear at the othe side of the screen.
 **/
template <typename T, typename U> static void wrapCoordinates(T& point_x, T& point_y, U screen_x, U screen_y);
template <typename T, typename U> static void wrapCoordinates(T& point, const U& screen_size);


/* DEFINITIONS BELOW */


/** Performs an axis-aligned bounding box collision detection.
 *  @returns true if there is a collision.
 **/
template <typename T>
bool checkCircleAABBCollision(T a_x, T a_y, T a_radius, T b_x, T b_y, T b_radius) {
  return (a_x + a_radius + b_radius > b_x
       && a_x < b_x + a_radius + b_radius
       && a_y + a_radius + b_radius > b_y
       && a_y < b_y + a_radius + b_radius);
}

/** Performs an axis-aligned bounding box collision detection.
 *  @returns true if there is a collision.
 **/
template <typename T, typename U>
bool checkCircleAABBCollision(const T& circle_a, U a_radius, const T& circle_b, U b_radius) {
  return checkCircleAABBCollision(circle_a.x, circle_a.y, a_radius, circle_b.x, circle_b.y, b_radius);
}

/** Checks if 2 circles are colliding by comparing the sum of 
 *  their radius to the distance between their centers.
 *  *Warning* It uses an sqrtf() call.
 *  @returns true if there is a collision.
 **/
template <typename T>
bool checkCirclesCollision(T a_x, T a_y, T a_radius, T b_x, T b_y,  T b_radius) {
  return isPointInsideCircle(a_x, a_y, a_radius + b_radius, b_x, b_y);
}

/** Checks if 2 circles are colliding by comparing the sum of 
 *  their radius to the distance between their centers.
 *  *Warning* It uses an sqrtf() call.
 *  @returns true if there is a collision.
 **/
template <typename T, typename U>
bool checkCirclesCollision(T circle_a, U a_radius, T circle_b, U b_radius) {
  return checkCirclesCollision(circle_a.x, circle_a.y, a_radius, circle_b.x, circle_b.y, b_radius);
}

/** Checks if a circle is colliding with the screen border,
 *  changing its position and deltaV accordingly if there is a collision.
 *  @returns true if there is a collision.
 **/
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

/** Checks if a circle is colliding with the screen border,
 *  changing its position and deltaV accordingly if there is a collision.
 *  @returns true if there is a collision.
 **/
template <typename T, typename U, typename V> 
bool circleScreenCollision(T& circle, U radius, T& delta, const V& screen_size) {
  return circleScreenCollision(circle.x, circle.y, radius, delta.x, delta.y, screen_size.x, screen_size.y);
}

/** Checks if a circle is out of the screen, even for a pixel.
 *  @returns true if the circle is out of the screen.
 **/
template <typename T, typename U>
bool checkCircleOutScreen(T circle_x, T circle_y, T radius, U screen_x, U screen_y) {
  return (circle_x - radius < 0 || circle_x + radius >= screen_x || circle_y - radius < 0 || circle_y + radius >= screen_y) ? true : false;
}

/** Checks if a circle is out of the screen, even for a pixel.
 *  @returns true if the circle is out of the screen.
 **/
template <typename T, typename U, typename V>
bool checkCircleOutScreen(const T& circle, U radius, const V& screen_size) {
  return checkCircleOutScreen(circle.x, circle.y, radius, screen_size.x, screen_size.y);
}

/** Calculates the distance between 2 points.
 *  *Warning* Calls sqrtf().
 *  @returns the distance between the 2 points.
 **/
template <typename T>
float distanceBetweenPoints(T a_x, T a_y, T b_x, T b_y) {
  return sqrtf((a_x - b_x) * (a_x - b_x) + (a_y - b_y) * (a_y - b_y));
}

/** Calculates the distance between 2 points.
 *  *Warning* Calls sqrtf().
 *  @returns the distance between the 2 points.
 **/
template <typename T>
float distanceBetweenPoints(const T& a, const T& b) {
  return distanceBetweenPoints(a.x, a.y, b.x, b.y);
}

/** Calculates an elastic collision between 2 circles.
 *  Updates the deltaV of the circles.
 **/
template <typename T>
void elasticCollision(T a_x, T a_y, T& a_dx, T& a_dy, T a_m, T b_x, T b_y, T& b_dx, T& b_dy, T b_m, T distance) {
  const float nx = (b_x - a_x) / distance;
  const float ny = (b_y - a_y) / distance;

  const float kx = a_dx - b_dx;
  const float ky = a_dy - b_dy;
  const float p = 2.f * (nx * kx + ny * ky) / (a_m + b_m);

  a_dx = a_dx - p * b_m * nx;
  a_dy = a_dy - p * b_m * ny;

  b_dx = b_dx + p * a_m * nx;
  b_dy = b_dy + p * a_m * ny;
}

/** Calculates an elastic collision between 2 circles.
 *  Updates the deltaV of the circles.
 *  *Warning* Calls sqrtf().
 **/
template <typename T>
void elasticCollision(T a_x, T a_y, T& a_dx, T& a_dy, T a_m, T b_x, T b_y, T& b_dx, T& b_dy, T b_m) {
  const float distance = ktp::distanceBetweenPoints(a_x, a_y, b_x, b_y);
  elasticCollision(a_x, a_y, a_dx, a_dy, a_m, b_x, b_y, b_dx, b_dy, b_m, distance);
}

/** Calculates an elastic collision between 2 circles.
 *  Updates the deltaV of the circles.
 *  *Warning* Calls sqrtf().
 **/
template <typename T, typename U>
void elasticCollision(const T& circle_a, T& a_delta, U a_mass, const T& circle_b, T&b_delta, U b_mass) {
  const float distance = ktp::distanceBetweenPoints(circle_a, circle_b);
  elasticCollision(circle_a.x, circle_a.y, a_delta.x, a_delta.y, a_mass, circle_b.x, circle_b.y, b_delta.x, b_delta.y, b_mass, distance);
}

/** Calculates an elastic collision between 2 circles.
 *  Updates the deltaV of the circles.
 **/
template <typename T, typename U>
void elasticCollision(const T& circle_a, T& a_delta, U a_mass, const T& circle_b, T&b_delta, U b_mass, U distance) {
  elasticCollision(circle_a.x, circle_a.y, a_delta.x, a_delta.y, a_mass, circle_b.x, circle_b.y, b_delta.x, b_delta.y, b_mass, distance);
}

/** Checks if a point is inside a circle.
 *  *Warning* Calls sqrtf().
 *  @returns ture if the point is inside the circle.
 **/
template <typename T>
bool isPointInsideCircle(T circle_x, T circle_y, T radius, T point_x, T point_y) {
  return distanceBetweenPoints(circle_x, circle_y, point_x, point_y) < radius;
}

/** Checks if a point is inside a circle.
 *  *Warning* Calls sqrtf().
 *  @returns ture if the point is inside the circle.
 **/
template <typename T, typename U> 
bool isPointInsideCircle(const T& circle, U radius, const T& point) {
  return distanceBetweenPoints(circle.x, circle.y, point.x, point.y) < radius;
}

/** Checks if a point is outside the screen an wraps its coordinates
 *  in order to make it appear at the othe side of the screen.
 **/
template <typename T, typename U>
void wrapCoordinates(T& point_x, T& point_y, U screen_x, U screen_y) {
  if (point_x < 0) {
    point_x = screen_x + point_x;
  } else if (point_x > screen_x - 1) {
    point_x = point_x - screen_x;
  }

  if (point_y < 0) {
    point_y = screen_y + point_y;
  } else if (point_y > screen_y - 1) {
    point_y = point_y - screen_y;
  }
}

/** Checks if a point is outside the screen an wraps its coordinates
 *  in order to make it appear at the othe side of the screen.
 **/
template <typename T, typename U>
void wrapCoordinates(T& point, const U& screen_size) {
  wrapCoordinates(point.x, point.y, screen_size.x, screen_size.y);
}

} // namespace ktp
#endif // AEROLITS_INCLUDE_COLLISIONS_H_