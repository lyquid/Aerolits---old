#ifndef AEROLITS_HEADERS_SPACE_OBJECTS_H_
#define AEROLITS_HEADERS_SPACE_OBJECTS_H_

#include <chrono>
#include <random>
#include <vector>
#include <SDL.h>

struct SpaceObject {
  std::vector<SDL_FPoint> shape_;
  SDL_FPoint delta_;
  unsigned int size_;
  static unsigned int count_;
  bool on_edge_;

  SpaceObject(float x, float y, float dx, float dy, unsigned int aerolite_size);
  SpaceObject(const SDL_Point& screen_size);
  SpaceObject(unsigned int aerolite_size, const SDL_Point& screen_size);

  bool checkCollision(const SpaceObject& target);
  
  void move(float delta_time, const SDL_Point& screen_size);
  
  void render(SDL_Renderer& renderer) const;

 private:
  
  void generateDelta();

  SDL_FPoint generatePosition(const SDL_Point& screen_size);

  void generateShape(SDL_FPoint where);
  
  void generateSize();

  void warpCoordinates(SDL_FPoint& point, const SDL_Point& screen_size);
};
#endif // AEROLITS_HEADERS_SPACE_OBJECTS_H_