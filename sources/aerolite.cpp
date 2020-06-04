#include "../headers/space_objects.h"

unsigned int Aerolite::count_ = 0u;

Aerolite::Aerolite(float x, float y, float dx, float dy, unsigned int aerolite_size):
  center_({x, y}),
  delta_({dx, dy}),
  on_edge_(false),
  size_(aerolite_size),
  radius_(static_cast<float>(size_) / 2.f) {

  generateCircleShape(center_);
  ++count_;
}

Aerolite::Aerolite(const SDL_Point& screen_size):
  center_(generatePosition(screen_size)),
  delta_(generateDelta()),
  on_edge_(false),
  size_(generateSize()),
  radius_(static_cast<float>(size_) / 2.f) {

  generateCircleShape(center_);
  ++count_;
}

Aerolite::Aerolite(const Aerolite& object) {
  center_  = object.center_;
  delta_   = object.delta_;
  on_edge_ = object.on_edge_;
  shape_   = object.shape_;
  size_    = object.size_;
  radius_  = object.radius_;
  ++count_;
}

bool Aerolite::checkCollision(const Aerolite& target) {
  bool collided = false;

  return collided;
}

void Aerolite::move(float delta_time, const SDL_Point& screen_size) {
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

  center_.x += delta_.x * delta_time;
  center_.y += delta_.y * delta_time;
  wrapCoordinates(center_, screen_size);
  for (auto& point: shape_) {
    point.x += delta_.x * delta_time;
    point.y += delta_.y * delta_time;
    // if (on_edge_) warpCoordinates(point, screen_size);
    wrapCoordinates(point, screen_size);
  }
}

void Aerolite::render(SDL_Renderer& renderer) const {
  for (const auto& point: shape_) {
    SDL_RenderDrawPointF(&renderer, point.x, point.y);
  }
}

/* PRIVATE */

SDL_FPoint Aerolite::generateDelta() {
  SDL_FPoint delta;
  std::mt19937 generator(std::chrono::system_clock::now().time_since_epoch().count());
  std::uniform_real_distribution<float> distribution_dx(-100.f, 100.f);
  delta.x = distribution_dx(generator);
  std::uniform_real_distribution<float> distribution_dy(-100.f, 100.f);
  delta.y = distribution_dy(generator);
  return delta;
}

SDL_FPoint Aerolite::generatePosition(const SDL_Point& screen_size) {
  SDL_FPoint where;
  std::mt19937 generator(std::chrono::system_clock::now().time_since_epoch().count());
  std::uniform_real_distribution<float> distribution_x(0.f, screen_size.x);
  where.x = distribution_x(generator);
  std::uniform_real_distribution<float> distribution_y(0.f, screen_size.y);
  where.y = distribution_y(generator);
  printf("where.x:\t%f\t where.y:\t%f\t\n", where.x, where.y); // without this line g++ shows random aerolites
  return where;
}

void Aerolite::generateSquareShape(const SDL_FPoint& where) {
  for (auto i = 0u; i < size_; ++i) {
    shape_.push_back({where.x + i, where.y});
    if (i >= 1u) {
      shape_.push_back({where.x + size_, where.y + i});
      shape_.push_back({where.x + i,     where.y + size_});
      shape_.push_back({where.x,         where.y + i});
    }
  }
}

void Aerolite::generateCircleShape(const SDL_FPoint& where) {
  const int32_t diameter = radius_ * 2;
  int32_t x = radius_ - 1;
  int32_t y = 0;
  int32_t tx = 1;
  int32_t ty = 1;
  int32_t error = (tx - diameter);

  while (x >= y) {
    shape_.push_back({where.x + x, where.y - y});
    shape_.push_back({where.x + x, where.y + y});
    shape_.push_back({where.x - x, where.y - y});
    shape_.push_back({where.x - x, where.y + y});
    shape_.push_back({where.x + y, where.y - x});
    shape_.push_back({where.x + y, where.y + x});
    shape_.push_back({where.x - y, where.y - x});
    shape_.push_back({where.x - y, where.y + x});

    if (error <= 0) {
      ++y;
      error += ty;
      ty += 2;
    }

    if (error > 0) {
      --x;
      tx += 2;
      error += (tx - diameter);
    }
  }
}

unsigned int Aerolite::generateSize() {
  return generateSize(60u, 140u);
}

unsigned int Aerolite::generateSize(unsigned int min, unsigned int max) {
  std::mt19937 generator(std::chrono::system_clock::now().time_since_epoch().count());
  std::uniform_int_distribution<unsigned int> distribution(min, max);
  return distribution(generator);
}

void Aerolite::wrapCoordinates(SDL_FPoint& point, const SDL_Point& screen_size) {
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
