#ifndef AEROLITS_HEADERS_SPACE_OBJECTS_H_
#define AEROLITS_HEADERS_SPACE_OBJECTS_H_

#include <chrono>
#include <random>
#include <SDL.h>

struct SpaceObject {
  float x;
  float y;
  float dx;
  float dy;
  float size;
  bool shadow_present;
  float shadow_x;
  float shadow_y;
  static unsigned int count;

  SpaceObject(float sz, const SDL_Point& screen_size);

  void calculateShadow(SDL_Rect& rect) const;  
  
  void move(float delta_time, const SDL_Point& screen_size);
  
  void render(SDL_Renderer* renderer) const;

  void toSDL_Rect(SDL_Rect& rect) const;
};

#endif // AEROLITS_HEADERS_SPACE_OBJECTS_H_