#ifndef AEROLITS_HEADERS_SPACE_OBJECTS_H_
#define AEROLITS_HEADERS_SPACE_OBJECTS_H_

#include <chrono>
#include <random>
#include <SDL.h>

#include "../include/log.h"

struct SpaceObject {
  float x;
  float y;
  float dx;
  float dy;
  float size;
  static unsigned int count;

  SpaceObject(float sz, const SDL_Point& screen_size);

  void move(float delta_time, const SDL_Point& screen_size);

  void render(SDL_Renderer* renderer) const;

  void toSDL_Rect(SDL_Rect& rect) const;
};

#endif // AEROLITS_HEADERS_SPACE_OBJECTS_H_