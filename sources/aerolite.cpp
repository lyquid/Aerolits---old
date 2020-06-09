#include "../headers/space_objects.h"

unsigned int Aerolite::count_ = 0u;

Aerolite::Aerolite(float x, float y, float dx, float dy, unsigned int aerolite_size):
  center_({x, y}),
  delta_({dx, dy}),
  size_(aerolite_size),
  radius_(static_cast<float>(size_) / 2.f),
  mass_(radius_ * 10.f) {

  generateCircleShape(center_);
  ++count_;
}

Aerolite::Aerolite(const SDL_Point& screen_size):
  center_(generatePosition(screen_size)),
  delta_(generateDelta()),
  size_(generateSize()),
  radius_(static_cast<float>(size_) / 2.f),
  mass_(radius_ * 10.f) {

  generateCircleShape(center_);
  ++count_;
}

Aerolite::Aerolite(const Aerolite& object) {
  center_  = object.center_;
  delta_   = object.delta_;
  shape_   = object.shape_;
  size_    = object.size_;
  radius_  = object.radius_;
  mass_    = object.mass_;
  ++count_;
}

void Aerolite::render(SDL_Renderer& renderer) const {
  for (const auto& point: shape_) {
    SDL_RenderDrawPointF(&renderer, point.x, point.y);
  }
  SDL_RenderDrawPointF(&renderer, center_.x, center_.y);
}

void Aerolite::updateAerolites(float delta_time, const SDL_Point& screen_size, std::vector<std::unique_ptr<Aerolite>>& aerolites) {
  // move all
  for (auto i = 0u; i < aerolites.size(); ++i) {
    aerolites[i]->center_.x += aerolites[i]->delta_.x * delta_time;
    aerolites[i]->center_.y += aerolites[i]->delta_.y * delta_time;
  }
  // check all
  for (auto i = 0u; i < aerolites.size(); ++i) {
    // wall collisions
    const bool wall_collision = ktp::circleScreenCollision(aerolites[i]->center_, aerolites[i]->radius_, aerolites[i]->delta_, screen_size);
    bool aero_collision;
    // the other aerolite collisions
    for (auto j = i + 1u; j < aerolites.size(); ++j) {
      if (i != j) { // not myself
        const bool aabb_check = ktp::checkCircleAABBCollision(aerolites[i]->radius_, aerolites[i]->center_.x, aerolites[i]->center_.y,
                                                              aerolites[j]->radius_, aerolites[j]->center_.x, aerolites[j]->center_.y);
        if (aabb_check) { // maybe a collision

          const float distance = ktp::distanceBetweenPoints(aerolites[i]->center_, aerolites[j]->center_);
          aero_collision = distance < aerolites[i]->radius_ + aerolites[j]->radius_;

          if (aero_collision) { // for sure it's a collision 

            const float nx = (aerolites[j]->center_.x - aerolites[i]->center_.x) / distance;
            const float ny = (aerolites[j]->center_.y - aerolites[i]->center_.y) / distance;

            const float kx = (aerolites[i]->delta_.x - aerolites[j]->delta_.x);
            const float ky = (aerolites[i]->delta_.y - aerolites[j]->delta_.y);
            const float p = 2.f * (nx * kx + ny * ky) / (aerolites[i]->mass_ + aerolites[j]->mass_);

            aerolites[i]->delta_.x = aerolites[i]->delta_.x - p * aerolites[j]->mass_ * nx;
            aerolites[i]->delta_.y = aerolites[i]->delta_.y - p * aerolites[j]->mass_ * ny;
            aerolites[j]->delta_.x = aerolites[j]->delta_.x + p * aerolites[i]->mass_ * nx;
            aerolites[j]->delta_.y = aerolites[j]->delta_.y + p * aerolites[i]->mass_ * ny;

            // move by new delta
            aerolites[i]->center_.x += aerolites[i]->delta_.x * delta_time;
            aerolites[i]->center_.y += aerolites[i]->delta_.y * delta_time;
            aerolites[j]->center_.x += aerolites[j]->delta_.x * delta_time;
            aerolites[j]->center_.y += aerolites[j]->delta_.y * delta_time;
          }
        }
      }
    }
    if (wall_collision || aero_collision) {
      aerolites[i]->generateCircleShape({aerolites[i]->center_.x, aerolites[i]->center_.y});
    } else {
      for (auto& point: aerolites[i]->shape_) {
        point.x += aerolites[i]->delta_.x * delta_time;
        point.y += aerolites[i]->delta_.y * delta_time;
      }
    }
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
  shape_.clear();

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
  return generateSize(60u, 200u);
}

unsigned int Aerolite::generateSize(unsigned int min, unsigned int max) {
  std::mt19937 generator(std::chrono::system_clock::now().time_since_epoch().count());
  std::uniform_int_distribution<unsigned int> distribution(min, max);
  return distribution(generator);
}
