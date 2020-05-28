#include "../headers/space_objects.h"

unsigned int SpaceObject::count_ = 0u;

SpaceObject::SpaceObject(float x, float y, float dx, float dy, float size):
  shadow_present_(false) {
  
  shape_.x = x;
  shape_.y = y;
  delta_.x = dx;
  delta_.y = dy;
  shape_.h = size;
  shape_.w = size;
  ++count_;
}

SpaceObject::SpaceObject(float aerolite_size, const SDL_Point& screen_size): 
  shadow_present_(false) {

  std::mt19937 generator(std::chrono::system_clock::now().time_since_epoch().count());

  std::uniform_real_distribution<float> distribution_x(0.f, screen_size.x);
  shape_.x = distribution_x(generator);
  std::uniform_real_distribution<float> distribution_y(0.f, screen_size.y);
  shape_.y = distribution_y(generator);
  std::uniform_real_distribution<float> distribution_dx(-100.f, 100.f);
  delta_.x = distribution_dx(generator);
  std::uniform_real_distribution<float> distribution_dy(-100.f, 100.f);
  delta_.y = distribution_dy(generator);

  shape_.h = aerolite_size;
  shape_.w = aerolite_size;
  ++count_;
}

bool SpaceObject::checkCollision(const SpaceObject* target) {
  bool collided = false;

  return collided;
}

void SpaceObject::getShadowRect(SDL_FRect& rect) const {
  rect.x = shadow_.x;
  rect.y = shadow_.y;
  rect.h = shape_.h;
  rect.w = shape_.w;
}

void SpaceObject::move(float delta_time, const SDL_Point& screen_size) {

  shadow_present_ = false;

  shape_.x += delta_.x * delta_time;
  shape_.y += delta_.y * delta_time;
 
  if (shape_.x < 0.f && delta_.x < 0.f) {
    shadow_present_ = true;
    shadow_.x = screen_size.x + shape_.x;
    shadow_.y = shape_.y;
    if (shape_.x + shape_.w < 0.f) {
      shape_.x = screen_size.x - shape_.w;
      shadow_present_ = false;
    }
  } else if (shape_.x + shape_.w > screen_size.x && delta_.x > 0.f) {
    shadow_present_ = true;
    shadow_.x = -(screen_size.x - shape_.x);
    shadow_.y = shape_.y;
    if (shape_.x > screen_size.x) {
      shape_.x = 0.f;
      shadow_present_ = false;
    }
  }

  if (shape_.y < 0.f && delta_.y < 0.f) {
    shadow_present_ = true;
    shadow_.y = screen_size.y + shape_.y;
    shadow_.x = shape_.x;
    if (shape_.y + shape_.h < 0.f) {
      shape_.y = screen_size.y - shape_.h;
      shadow_present_ = false;
    }
  } else if (shape_.y + shape_.h > screen_size.y && delta_.y > 0.f) {
    shadow_present_ = true;
    shadow_.y = -(screen_size.y - shape_.y);
    shadow_.x = shape_.x;
    if (shape_.y > screen_size.y) {
      shape_.y = 0.f;
      shadow_present_ = false;
    }
  }
}

void SpaceObject::render(SDL_Renderer* renderer) const {
  SDL_RenderDrawRectF(renderer, &shape_);
  if (shadow_present_) {
    SDL_FRect shadow_rect;
    getShadowRect(shadow_rect);
    SDL_RenderDrawRectF(renderer, &shadow_rect);
  }
}
