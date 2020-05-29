#include "../headers/space_objects.h"

unsigned int SpaceObject::count_ = 0u;

SpaceObject::SpaceObject(float x, float y, float dx, float dy, unsigned int aerolite_size) {
  size_ = aerolite_size;
  generateShape({x, y});
  delta_.x = dx;
  delta_.y = dy; 
  on_edge_ = false;
  ++count_;
}

SpaceObject::SpaceObject(const SDL_Point& screen_size) {
  generateSize();
  generateShape(generatePosition(screen_size));
  generateDelta();
  on_edge_ = false;
  ++count_;
}

SpaceObject::SpaceObject(unsigned int aerolite_size, const SDL_Point& screen_size) {
  size_ = aerolite_size;
  generateShape(generatePosition(screen_size));
  generateDelta();
  on_edge_ = false;
  ++count_;
}

bool SpaceObject::checkCollision(const SpaceObject& target) {
  bool collided = false;

  return collided;
}

void SpaceObject::move(float delta_time, const SDL_Point& screen_size) {
  // trying to just wapr coordinates only on shapes at the edges
  // not working properly
  /* SDL_Rect test;
  test.x = shape_.front().x + delta_.x * delta_time;
  test.y = shape_.front().y + delta_.y * delta_time;
  test.h = size_;
  test.w = size_;
  on_edge_ = false;

  if (test.x < 0.f) {
    on_edge_ = true;
  } else if (test.x + test.w > screen_size.x - 1) {
    on_edge_ = true;
  } else if (test.y < 0.f) {
    on_edge_ = true;
  } else  if (test.y + test.h > screen_size.y - 1) {
    on_edge_ = true;
  } */

  for (auto& point: shape_) {
    point.x += delta_.x * delta_time;
    point.y += delta_.y * delta_time;
    // if (on_edge_) warpCoordinates(point, screen_size);
    warpCoordinates(point, screen_size);
  }
}

void SpaceObject::render(SDL_Renderer& renderer) const {
  for (const auto& point: shape_) {
    SDL_RenderDrawPointF(&renderer, point.x, point.y);
  }
}

/* PRIVATE */

void SpaceObject::generateDelta() {
  std::mt19937 generator(std::chrono::system_clock::now().time_since_epoch().count());
  std::uniform_real_distribution<float> distribution_dx(-100.f, 100.f);
  delta_.x = distribution_dx(generator);
  std::uniform_real_distribution<float> distribution_dy(-100.f, 100.f);
  delta_.y = distribution_dy(generator);
}

SDL_FPoint SpaceObject::generatePosition(const SDL_Point& screen_size) {
  SDL_FPoint where;
  std::mt19937 generator(std::chrono::system_clock::now().time_since_epoch().count());
  std::uniform_real_distribution<float> distribution_x(0.f, screen_size.x);
  where.x = distribution_x(generator);
  std::uniform_real_distribution<float> distribution_y(0.f, screen_size.y);
  where.y = distribution_y(generator);
  return where;
}

void SpaceObject::generateShape(SDL_FPoint where) {
  SDL_FPoint copy_point;
  // top
  for (auto i = 0u; i < size_; ++i) {
    copy_point.x = where.x + i;
    copy_point.y = where.y;
    shape_.push_back(copy_point);
  }
  // right
  for (auto i = 1u; i < size_; ++i) {
    copy_point.x = where.x + size_;
    copy_point.y = where.y + i;
    shape_.push_back(copy_point);
  }
  // bottom
  for (auto i = 1u; i < size_; ++i) {
    copy_point.x = where.x + i;
    copy_point.y = where.y + size_;
    shape_.push_back(copy_point);
  }
  // left
  for (auto i = 1u; i < size_; ++i) {
    copy_point.x = where.x;
    copy_point.y = where.y + i;
    shape_.push_back(copy_point);
  }
}

void SpaceObject::generateSize() {
  std::mt19937 generator(std::chrono::system_clock::now().time_since_epoch().count());
  std::uniform_int_distribution<unsigned int> distribution(70u, 130u);
  size_ = distribution(generator);
}

void SpaceObject::warpCoordinates(SDL_FPoint& point, const SDL_Point& screen_size) {
  if (point.x < 0) {
    point.x = screen_size.x + point.x;
  } else if (point.x > screen_size.x - 1u) {
    point.x = point.x - screen_size.x;
  }

  if (point.y < 0) {
    point.y = screen_size.y + point.y;
  } else if (point.y > screen_size.y - 1u) {
    point.y = point.y - screen_size.y;
  }
}
