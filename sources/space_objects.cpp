#include "../headers/space_objects.h"

unsigned int SpaceObject::count_ = 0u;

SpaceObject::SpaceObject(float x, float y, float dx, float dy, unsigned int aerolite_size) {
  size_ = aerolite_size;
  radius_ = static_cast<float>(size_) / 2.f;
  generateSquareShape({x, y});
  delta_.x = dx;
  delta_.y = dy; 
  on_edge_ = false;
  ++count_;
}

SpaceObject::SpaceObject(SpaceObjectType type, float x, float y, float dx, float dy, unsigned int aerolite_size) {
  size_ = aerolite_size;
  radius_ = static_cast<float>(size_) / 2.f;
  switch (type) {
    case SpaceObjectType::Square:
      generateSquareShape({x, y});
      break;
    case SpaceObjectType::Round:
      generateCircleShape({x, y});
      break;
  }
  delta_.x = dx;
  delta_.y = dy; 
  on_edge_ = false;
  ++count_;
}

SpaceObject::SpaceObject(const SDL_Point& screen_size) {
  generateSize(60, 140);
  radius_ = static_cast<float>(size_) / 2.f;
  // generateSquareShape(generatePosition(screen_size));
  generateCircleShape(generatePosition(screen_size));
  generateDelta();
  on_edge_ = false;
  ++count_;
}

SpaceObject::SpaceObject(unsigned int aerolite_size, const SDL_Point& screen_size) {
  size_ = aerolite_size;
  radius_ = static_cast<float>(size_) / 2.f;
  generateSquareShape(generatePosition(screen_size));
  generateDelta();
  on_edge_ = false;
  ++count_;
}

SpaceObject::SpaceObject(const SpaceObject& object) {
  size_ = object.size_;
  radius_ = object.radius_;
  shape_ = object.shape_;
  delta_ = object.delta_;
  on_edge_ = object.on_edge_;
  ++count_;
}

bool SpaceObject::checkCollision(const SpaceObject& target) {
  bool collided = false;

  return collided;
}

void SpaceObject::move(float delta_time, const SDL_Point& screen_size, std::vector<std::unique_ptr<SpaceObject>>& aerolites) {
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

  for (auto it = shape_.begin(); it != shape_.end(); ++it) {

    it->x += delta_.x * delta_time;
    if (it == shape_.end()) { // carefull with this. Maybe: std::next(it) == shape_.end()
      for (auto& aerolite: aerolites) {
        checkCollision(*aerolite);
      }
    }

    it->y += delta_.y * delta_time;
    if (it == shape_.end()) {
      for (auto& aerolite: aerolites) {
        checkCollision(*aerolite);
      }
    }

    // if (on_edge_) warpCoordinates(point, screen_size);
    warpCoordinates(*it, screen_size);
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

void SpaceObject::generateSquareShape(const SDL_FPoint& where) {
  for (auto i = 0u; i < size_; ++i) {
    shape_.push_back({where.x + i, where.y});
    if (i >= 1u) {
      shape_.push_back({where.x + size_, where.y + i});
      shape_.push_back({where.x + i,     where.y + size_});
      shape_.push_back({where.x,         where.y + i});
    }
  }
}

void SpaceObject::generateCircleShape(const SDL_FPoint& where) {
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

void SpaceObject::generateSize() {
  generateSize(70u, 130u);
}

void SpaceObject::generateSize(unsigned int min, unsigned int max) {
  std::mt19937 generator(std::chrono::system_clock::now().time_since_epoch().count());
  std::uniform_int_distribution<unsigned int> distribution(min, max);
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
