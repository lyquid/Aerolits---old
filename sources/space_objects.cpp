#include "../headers/space_objects.h"

unsigned int SpaceObject::count = 0u;

SpaceObject::SpaceObject(float aerolite_size, const SDL_Point& screen_size): 
  shadow_present(false) {

  const auto seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::mt19937 generator(seed);

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

void SpaceObject::calculateShadow(SDL_Rect& rect) const {
  rect.x = shadow_x;
  rect.y = shadow_y;
  rect.h = size;
  rect.w = size;
}

void SpaceObject::move(float delta_time, const SDL_Point& screen_size) {

  shadow_present = false;

  x += dx * delta_time;
  y += dy * delta_time;

  shadow_x = x;
  shadow_y = y;
 
  if (x < 0.f && dx < 0.f) {
    shadow_present = true;
    shadow_x = screen_size.x + x;
    if (x + size < 0.f) {
      x = screen_size.x - size;
      shadow_present = false;
    }
  } else if (x + size > screen_size.x && dx > 0.f) {
    shadow_present = true;
    shadow_x = -(screen_size.x - x);
    if (x > screen_size.x) {
      x = 0.f;
      shadow_present = false;
    }
  }

  if (y < 0.f && dy < 0.f) {
    shadow_present = true;
    shadow_y = screen_size.y + y;
    if (y + size < 0.f) {
      y = screen_size.y - size;
      shadow_present = false;
    }
  } else if (y + size > screen_size.y && dy > 0.f) {
    shadow_present = true;
    shadow_y = -(screen_size.y - y);
    if (y > screen_size.y) {
      y = 0.f;
      shadow_present = false;
    }
  }

}

void SpaceObject::render(SDL_Renderer* renderer) const {
  SDL_Rect shape, shadow_rect;
  toSDL_Rect(shape);
  SDL_RenderDrawRect(renderer, &shape);
  if (shadow_present) {
    calculateShadow(shadow_rect);
    SDL_RenderDrawRect(renderer, &shadow_rect);
  }
}

void SpaceObject::toSDL_Rect(SDL_Rect& rect) const {
  rect.x = x;
  rect.y = y;
  rect.h = size;
  rect.w = size;
}