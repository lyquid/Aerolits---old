#ifndef AEROLITS_HEADERS_SPACE_OBJECTS_H_
#define AEROLITS_HEADERS_SPACE_OBJECTS_H_

#include <chrono>
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
  float angle_;
  SDL_FPoint center_;

  Player(const SDL_Point& screen_size);

  void render(SDL_Renderer& renderer) const;
 
 private:
 
  void generatePlayerShape(const SDL_FPoint& where);

};
#endif // AEROLITS_HEADERS_SPACE_OBJECTS_H_