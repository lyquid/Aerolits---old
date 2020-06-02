#ifndef AEROLITS_HEADERS_SPACE_OBJECTS_H_
#define AEROLITS_HEADERS_SPACE_OBJECTS_H_

#include <chrono>
#include <cmath>
#include <random>
#include <vector>
#include <SDL.h>

enum class SpaceObjectType { 
  Square,
  Round,
  Ship,
  count
};

struct SpaceObject {
  std::vector<SDL_FPoint> shape_;
  SDL_FPoint              delta_;
  unsigned int            size_;
  float                   radius_;

  SpaceObject():
  shape_(),
  delta_({0.f, 0.f}),
  size_(0u),
  radius_(0.f) {}
};

struct Aerolite : SpaceObject {
  bool on_edge_;
  static unsigned int count_;

  Aerolite(float x, float y, float dx, float dy, unsigned int aerolite_size);
  Aerolite(SpaceObjectType type, float x, float y, float dx, float dy, unsigned int aerolite_size);
  Aerolite(const SDL_Point& screen_size);
  Aerolite(unsigned int aerolite_size, const SDL_Point& screen_size);
  Aerolite(const Aerolite& object);

  bool checkCollision(const Aerolite& target);
  
  void move(float delta_time, const SDL_Point& screen_size);
  
  void render(SDL_Renderer& renderer) const;

 private:
  
  void generateDelta();

  SDL_FPoint generatePosition(const SDL_Point& screen_size);

  void generateSquareShape(const SDL_FPoint& where);

  void generateCircleShape(const SDL_FPoint& where);
  
  void generateSize();

  void generateSize(unsigned int min, unsigned int max);

  void warpCoordinates(SDL_FPoint& point, const SDL_Point& screen_size);
};

struct Player : SpaceObject {

  Player(const SDL_Point& screen_size);

  void move(float delta_time);
  void render(SDL_Renderer& renderer) const;
  void thrust(float delta_time);
  void shoot(float delta_time);
  void steerLeft(float delta_time);
  void steerRight(float delta_time);
 
 private:
  float angle_;
  SDL_FPoint center_;
  float size_;
  std::vector<SDL_FPoint> render_shape_;
  std::vector<std::vector<SDL_FPoint>> render_shape_clones_;

  const SDL_Point kSCREEN_SIZE_;

  void copyClones();
  void generatePlayerShape();
  void resizeClones();
  void rotatePlayer();

};
#endif // AEROLITS_HEADERS_SPACE_OBJECTS_H_