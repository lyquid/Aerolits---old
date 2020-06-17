#ifndef AEROLITS_HEADERS_SPACE_OBJECTS_H_
#define AEROLITS_HEADERS_SPACE_OBJECTS_H_

#include <chrono>
#include <cmath>
#include <memory>
#include <random>
#include <vector>
#include <SDL.h>

#include "../include/collisions.h"
#include "../include/timer.h"

enum class SpaceObjectType { 
  Square,
  Round,
  Ship,
  count
};

struct Aerolite {
  
  Aerolite(float x, float y, float dx, float dy, unsigned int aerolite_size);
  Aerolite(const SDL_Point& screen_size);
  Aerolite(const Aerolite& object);
  ~Aerolite() { --count_; }

  void render(SDL_Renderer& renderer) const;
  static void updateAerolites(float delta_time, const SDL_Point& screen_size, std::vector<std::unique_ptr<Aerolite>>& aerolites);

  static unsigned int     count_;
  SDL_FPoint              center_;
  SDL_FPoint              delta_;
  std::vector<SDL_FPoint> shape_;
  unsigned int            size_;
  float                   radius_;
  float                   mass_;
  bool                    wraping_;
  std::vector<SDL_FPoint> wraping_clones_;
  
 private:
  static float calculateMass(float radius, float density = 0.0037f);
  static SDL_FPoint generateDelta(float min = -100.f, float max = 100.f);
  static SDL_FPoint generatePosition(const SDL_Point& screen_size);
  void generateSquareShape(const SDL_FPoint& where);
  void generateCircleShape(const SDL_FPoint& where);
  static unsigned int generateSize(unsigned int min = 90u, unsigned int max = 100u);
  void generateWrapingClones(const SDL_Point& screen_size);
  void updateDeltas(float delta_time);
};

struct Bullet {

  SDL_FPoint delta_;
  std::vector<SDL_FPoint> shape_;

  Bullet(SDL_FPoint where, SDL_FPoint delta): 
    delta_({delta.x, delta.y}) {

    shape_.resize(9);
    /* clockwise square 
        8  1  2
        7  0  3
        6  5  4    */
    shape_[0].x = where.x;
    shape_[0].y = where.y;
    shape_[1].x = where.x;
    shape_[1].y = where.y - 1;
    shape_[2].x = where.x + 1;
    shape_[2].y = where.y - 1;
    shape_[3].x = where.x + 1;
    shape_[3].y = where.y;
    shape_[4].x = where.x + 1;
    shape_[4].y = where.y + 1;
    shape_[5].x = where.x;
    shape_[5].y = where.y + 1;
    shape_[6].x = where.x - 1;
    shape_[6].y = where.y + 1;
    shape_[7].x = where.x - 1;
    shape_[7].y = where.y;
    shape_[8].x = where.x - 1;
    shape_[8].y = where.y - 1;
  }
};

struct Player {

  Player(const SDL_Point& screen_size);

  bool isAlive() { return alive_; }
  void render(SDL_Renderer& renderer) const;
  void reset();
  void shoot(float delta_time);
  void steerLeft(float delta_time) { angle_ -= 5.f * delta_time; }
  void steerRight(float delta_time) { angle_ += 5.f * delta_time; }
  void thrust(float delta_time);
  void update(float delta_time, std::vector<std::unique_ptr<Aerolite>>& aerolites);
 
 private:

  bool                                 alive_;
  float                                angle_;
  SDL_FPoint                           center_;
  SDL_FPoint                           delta_;
  float                                size_;
  std::vector<SDL_FPoint>              render_shape_;
  std::vector<std::vector<SDL_FPoint>> render_shape_clones_;
  std::vector<SDL_FPoint>              shape_;
  ktp::Timer                           shooting_timer_;
  std::vector<Bullet>                  bullets_;

  const SDL_Point kSCREEN_SIZE_;

  bool checkPlayerCollisions(const std::vector<std::unique_ptr<Aerolite>>& aerolites);
  bool checkBulletsCollisions(const Bullet& bullet, std::vector<std::unique_ptr<Aerolite>>& aerolites);
  void copyClones();
  void generatePlayerShape();
  void move(float delta_time);
  void resizeClones();
  void rotate();
  void updateBullets(float delta_time, std::vector<std::unique_ptr<Aerolite>>& aerolites);
};

#endif // AEROLITS_HEADERS_SPACE_OBJECTS_H_