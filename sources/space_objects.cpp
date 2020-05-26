#include "../headers/space_objects.h"

unsigned int SpaceObject::count = 0u;

SpaceObject::SpaceObject(float aerolite_size, const SDL_Point& screen_size) {

  const auto seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine generator(seed);

  std::uniform_real_distribution<float> distribution_x(0.f, screen_size.x);
  x = distribution_x(generator);
  std::uniform_real_distribution<float> distribution_y(0.f, screen_size.y);
  y = distribution_y(generator);
  std::uniform_real_distribution<float> distribution_dx(-100.f, 100.f);
  dx = distribution_dx(generator);
  std::uniform_real_distribution<float> distribution_dy(-100.f, 100.f);
  dy = distribution_dy(generator);

  size = aerolite_size;
  ++count;
}

void SpaceObject::move(float delta_time, const SDL_Point& screen_size) {
  x += dx * delta_time;
  if (x + size < 0) {
    x = screen_size.x;
  } else if (x > screen_size.x) {
    x = 0.f;
  }

  y += dy * delta_time;
  if (y + size < 0 ) {
    y = screen_size.y;
  } else if (y > screen_size.y) {
    y = 0.f;
  }
}

void SpaceObject::render(SDL_Renderer* renderer) const {
  SDL_Rect shape;
  toSDL_Rect(shape);
  SDL_RenderDrawRect(renderer, &shape);
}

void SpaceObject::toSDL_Rect(SDL_Rect& rect) const {
  rect.x = x;
  rect.y = y;
  rect.h = size;
  rect.w = size;
}