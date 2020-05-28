#ifndef AEROLITS_HEADERS_SPACE_OBJECTS_H_
#define AEROLITS_HEADERS_SPACE_OBJECTS_H_

#include <chrono>
#include <random>
#include <SDL.h>

struct SpaceObject {
  SDL_FRect shape_;
  SDL_FPoint delta_;
  bool shadow_present_;
  SDL_FPoint shadow_;
  static unsigned int count_;

  SpaceObject(float x, float y, float dx, float dy, float size);
  SpaceObject(float aerolite_size, const SDL_Point& screen_size);

  bool checkCollision(const SpaceObject* target);

  void getShadowRect(SDL_FRect& rect) const;
  
  void move(float delta_time, const SDL_Point& screen_size);
  
  void render(SDL_Renderer* renderer) const;

};

#endif // AEROLITS_HEADERS_SPACE_OBJECTS_H_