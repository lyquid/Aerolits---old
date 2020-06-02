#ifndef AEROLITS_HEADERS_SPACE_OBJECTS_H_
#define AEROLITS_HEADERS_SPACE_OBJECTS_H_

#include <chrono>
#include <cmath>
#include <list>
#include <random>
#include <vector>
#include <SDL.h>

enum class SpaceObjectType { 
  Square,
  Round,
  Ship,
  count
};

struct Aerolite {
  
  static unsigned int     count_;
  SDL_FPoint              delta_;
  bool                    on_edge_;
  std::vector<SDL_FPoint> shape_;
  unsigned int            size_;
  float                   radius_;
  
  Aerolite(const SDL_Point& screen_size);
  Aerolite(const Aerolite& object);
  Aerolite(float x, float y, float dx, float dy, unsigned int aerolite_size);
  ~Aerolite() { --count_; }

  bool checkCollision(const Aerolite& target);
  void move(float delta_time, const SDL_Point& screen_size);
  void render(SDL_Renderer& renderer) const;

 private:
  
  SDL_FPoint generateDelta();
  SDL_FPoint generatePosition(const SDL_Point& screen_size);
  void generateSquareShape(const SDL_FPoint& where);
  void generateCircleShape(const SDL_FPoint& where);
  unsigned int generateSize();
  unsigned int generateSize(unsigned int min, unsigned int max);
  void wrapCoordinates(SDL_FPoint& point, const SDL_Point& screen_size);
};

struct Bullet {

  SDL_FPoint delta_;
  // std::vector<SDL_FPoint> shape_; // will see
  SDL_FPoint shape_;
  float size_;

  Bullet(SDL_FPoint where, SDL_FPoint delta): 
    size_(5.f),
    shape_({where.x, where.y}),
    delta_({delta.x, delta.y}) {}
};

struct Player {

  SDL_FPoint              delta_;
  float                   radius_;
  std::vector<SDL_FPoint> shape_;

  Player(const SDL_Point& screen_size);

  void render(SDL_Renderer& renderer) const;
  void thrust(float delta_time);
  void shoot(float delta_time);
  void steerLeft(float delta_time);
  void steerRight(float delta_time);
  void update(float delta_time);
 
 private:

  float                                angle_;
  SDL_FPoint                           center_;
  float                                size_;
  std::vector<SDL_FPoint>              render_shape_;
  std::vector<std::vector<SDL_FPoint>> render_shape_clones_;
  std::list<Bullet>                    bullets_;

  const SDL_Point kSCREEN_SIZE_;

  void copyClones();
  void generatePlayerShape();
  void move(float delta_time);
  void resizeClones();
  void rotate();
  void updateBullets(float delta_time);
};

#endif // AEROLITS_HEADERS_SPACE_OBJECTS_H_