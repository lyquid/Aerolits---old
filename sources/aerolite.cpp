#include "../headers/space_objects.h"

unsigned int Aerolite::count_ = 0u;

Aerolite::Aerolite(float x, float y, float dx, float dy, unsigned int aerolite_size):
  center_({x, y}),
  delta_({dx, dy}),
  size_(aerolite_size),
  radius_(static_cast<float>(size_) / 2.f),
  mass_(calculateMass(radius_)),
  wraping_(false) {

  generateCircleShape(center_);
  wraping_clones_.resize(3);
  ++count_;
}

Aerolite::Aerolite(const SDL_Point& screen_size):
  center_(generatePosition(screen_size)),
  delta_(generateDelta()),
  size_(generateSize()),
  radius_(static_cast<float>(size_) / 2.f),
  mass_(calculateMass(radius_)),
  wraping_(false) {
  
  generateCircleShape(center_);
  wraping_clones_.resize(3);
  ++count_;
}

Aerolite::Aerolite(const Aerolite& object) {
  center_            = object.center_;
  delta_             = object.delta_;
  shape_             = object.shape_;
  size_              = object.size_;
  radius_            = object.radius_;
  mass_              = object.mass_;
  wraping_           = object.wraping_;
  wraping_clones_    = object.wraping_clones_;
  ++count_;
}

void Aerolite::render(SDL_Renderer& renderer) const {
  SDL_SetRenderDrawColor(&renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  for (const auto& point: shape_) {
    SDL_RenderDrawPointF(&renderer, point.x, point.y);
  }
  // SDL_RenderDrawPointF(&renderer, center_.x, center_.y);
}

void Aerolite::updateAerolites(float delta_time, const SDL_Point& screen_size, std::vector<std::unique_ptr<Aerolite>>& aerolites) {
  // move all
  for (auto i = 0u; i < aerolites.size(); ++i) {
    aerolites[i]->center_.x += aerolites[i]->delta_.x * delta_time;
    aerolites[i]->center_.y += aerolites[i]->delta_.y * delta_time;
    aerolites[i]->wraping_ = ktp::checkCircleOutScreen(aerolites[i]->center_, aerolites[i]->radius_, screen_size);
    if (aerolites[i]->wraping_) aerolites[i]->generateWrapingClones(screen_size);
    ktp::wrapCoordinates(aerolites[i]->center_, screen_size);
  }

  for (auto i = 0u; i < aerolites.size(); ++i) {
    bool aero_collision = false;
    // the other aerolite collisions
    for (auto j = i + 1u; j < aerolites.size(); ++j) {
      if (i != j) { // not myself
        bool aabb_check = ktp::checkCircleAABBCollision(aerolites[i]->center_, aerolites[i]->radius_, aerolites[j]->center_, aerolites[j]->radius_);
        if (aabb_check) { // maybe a collision
          const float distance = ktp::distanceBetweenPoints(aerolites[i]->center_, aerolites[j]->center_);
          aero_collision = distance < aerolites[i]->radius_ + aerolites[j]->radius_;
          if (aero_collision) { // for sure it's a collision
            ktp::elasticCollision(aerolites[i]->center_, aerolites[i]->delta_, aerolites[i]->mass_,
                                  aerolites[j]->center_, aerolites[j]->delta_, aerolites[j]->mass_, distance);
            // move by new delta
            aerolites[i]->center_.x += aerolites[i]->delta_.x * delta_time;
            aerolites[i]->center_.y += aerolites[i]->delta_.y * delta_time;
            aerolites[j]->center_.x += aerolites[j]->delta_.x * delta_time;
            aerolites[j]->center_.y += aerolites[j]->delta_.y * delta_time;
            if (aerolites[i]->wraping_) {
              for (auto& clone: aerolites[i]->wraping_clones_) {
                clone.x += aerolites[i]->delta_.x * delta_time;
                clone.y += aerolites[i]->delta_.y * delta_time;
              }
            }
            if (aerolites[j]->wraping_) {
              for (auto& clone: aerolites[j]->wraping_clones_) {
                clone.x += aerolites[j]->delta_.x * delta_time;
                clone.y += aerolites[j]->delta_.y * delta_time;
              }
            }
          }
        }

        if (aerolites[j]->wraping_) {
          for (auto& clone: aerolites[j]->wraping_clones_) {
            aabb_check = ktp::checkCircleAABBCollision(aerolites[i]->center_, aerolites[i]->radius_, clone, aerolites[j]->radius_);
            if (aabb_check) {
              const float distance = ktp::distanceBetweenPoints(aerolites[i]->center_, clone);
              aero_collision = distance < aerolites[i]->radius_ + aerolites[j]->radius_;
              if (aero_collision) {
                ktp::elasticCollision(aerolites[i]->center_, aerolites[i]->delta_, aerolites[i]->mass_,
                                                      clone, aerolites[j]->delta_, aerolites[j]->mass_, distance);
                aerolites[i]->center_.x += aerolites[i]->delta_.x * delta_time;
                aerolites[i]->center_.y += aerolites[i]->delta_.y * delta_time;
                aerolites[j]->center_.x += aerolites[j]->delta_.x * delta_time;
                aerolites[j]->center_.y += aerolites[j]->delta_.y * delta_time;
                if (aerolites[i]->wraping_) {
                  for (auto& clone: aerolites[i]->wraping_clones_) {
                    clone.x += aerolites[i]->delta_.x * delta_time;
                    clone.y += aerolites[i]->delta_.y * delta_time;
                  }
                }
                if (aerolites[j]->wraping_) {
                  for (auto& clone: aerolites[j]->wraping_clones_) {
                    clone.x += aerolites[j]->delta_.x * delta_time;
                    clone.y += aerolites[j]->delta_.y * delta_time;
                  }
                }
              }
            }
          }
        }
      }
    }
    
    if (aerolites[i]->wraping_) {
      for (auto& clone: aerolites[i]->wraping_clones_) {
        for (auto j = i + 1u; j < aerolites.size(); ++j) {
          if (i != j) {
            bool aabb_clones_check = ktp::checkCircleAABBCollision(clone, aerolites[i]->radius_, aerolites[j]->center_, aerolites[j]->radius_);
            if (aabb_clones_check) {
              const float distance = ktp::distanceBetweenPoints(clone, aerolites[j]->center_);
              aero_collision = distance < aerolites[i]->radius_ + aerolites[j]->radius_;
              if (aero_collision) {
                ktp::elasticCollision(                clone, aerolites[i]->delta_, aerolites[i]->mass_,
                                      aerolites[j]->center_, aerolites[j]->delta_, aerolites[j]->mass_, distance);
                aerolites[i]->center_.x += aerolites[i]->delta_.x * delta_time;
                aerolites[i]->center_.y += aerolites[i]->delta_.y * delta_time;
                aerolites[j]->center_.x += aerolites[j]->delta_.x * delta_time;
                aerolites[j]->center_.y += aerolites[j]->delta_.y * delta_time;
                if (aerolites[i]->wraping_) {
                  for (auto& clone: aerolites[i]->wraping_clones_) {
                    clone.x += aerolites[i]->delta_.x * delta_time;
                    clone.y += aerolites[i]->delta_.y * delta_time;
                  }
                }
                if (aerolites[j]->wraping_) {
                  for (auto& clone: aerolites[j]->wraping_clones_) {
                    clone.x += aerolites[j]->delta_.x * delta_time;
                    clone.y += aerolites[j]->delta_.y * delta_time;
                  }
                }
              }
            }

            if (aerolites[j]->wraping_) {
              for (auto& t_clone: aerolites[j]->wraping_clones_) {
                aabb_clones_check = ktp::checkCircleAABBCollision(clone, aerolites[i]->radius_, t_clone, aerolites[j]->radius_);
                if (aabb_clones_check) {
                  const float distance = ktp::distanceBetweenPoints(clone, t_clone);
                  aero_collision = distance < aerolites[i]->radius_ + aerolites[j]->radius_;
                  if (aero_collision) {
                    ktp::elasticCollision(  clone, aerolites[i]->delta_, aerolites[i]->mass_,
                                          t_clone, aerolites[j]->delta_, aerolites[j]->mass_, distance);
                    aerolites[i]->center_.x += aerolites[i]->delta_.x * delta_time;
                    aerolites[i]->center_.y += aerolites[i]->delta_.y * delta_time;
                    aerolites[j]->center_.x += aerolites[j]->delta_.x * delta_time;
                    aerolites[j]->center_.y += aerolites[j]->delta_.y * delta_time;
                    if (aerolites[i]->wraping_) {
                      for (auto& clone: aerolites[i]->wraping_clones_) {
                        clone.x += aerolites[i]->delta_.x * delta_time;
                        clone.y += aerolites[i]->delta_.y * delta_time;
                      }
                    }
                    if (aerolites[j]->wraping_) {
                      for (auto& clone: aerolites[j]->wraping_clones_) {
                        clone.x += aerolites[j]->delta_.x * delta_time;
                        clone.y += aerolites[j]->delta_.y * delta_time;
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
    // move the circle shape to position
    aerolites[i]->generateCircleShape({aerolites[i]->center_.x, aerolites[i]->center_.y});
    for (auto& point: aerolites[i]->shape_) {
      ktp::wrapCoordinates(point, screen_size);
    }
  }
}

/* PRIVATE */

float Aerolite::calculateMass(float radius) {
  const auto volume = (4.f / 3.f) * M_PI * std::pow(radius, 3);
  constexpr auto density = 0.0037f;  // in kg/cm3
  return density * volume;
}

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

void Aerolite::generateWrapingClones(const SDL_Point& screen_size) {
/*  ___________
    | 0  | 1  |
    |____|____|
    | 2  | 3  |
    |____|____|  */
  if (center_.x < screen_size.x / 2 && center_.y < screen_size.y / 2) {
    // original in sector 0
    // clone in sector 1
    wraping_clones_[0].x = center_.x + screen_size.x;
    wraping_clones_[0].y = center_.y;
    // clone in sector 2
    wraping_clones_[1].x = center_.x;
    wraping_clones_[1].y = center_.y + screen_size.y;
    // clone in sector 3
    wraping_clones_[2].x = center_.x + screen_size.x;
    wraping_clones_[2].y = center_.y + screen_size.y;

  } else if (center_.x >= screen_size.x / 2 && center_.y < screen_size.y / 2) {
    // original in sector 1
    // clone in sector 0
    wraping_clones_[0].x = center_.x - screen_size.x;
    wraping_clones_[0].y = center_.y;
    // clone in sector 2
    wraping_clones_[1].x = center_.x - screen_size.x;
    wraping_clones_[1].y = center_.y + screen_size.y;
    // clone in sector 3
    wraping_clones_[2].x = center_.x;
    wraping_clones_[2].y = center_.y + screen_size.y;

  } else if (center_.x < screen_size.x / 2 && center_.y >= screen_size.y / 2) {
    // original in sector 2
    // clone in sector 0
    wraping_clones_[0].x = center_.x;
    wraping_clones_[0].y = center_.y - screen_size.y;
    // clone in sector 1
    wraping_clones_[1].x = center_.x + screen_size.x;
    wraping_clones_[1].y = center_.y - screen_size.y;
    // clone in sector 3
    wraping_clones_[2].x = center_.x + screen_size.x;
    wraping_clones_[2].y = center_.y;

  } else {
    // original in sector 3
    // clone in sector 0
    wraping_clones_[0].x = center_.x - screen_size.x;
    wraping_clones_[0].y = center_.y - screen_size.y;
    // clone in sector 1
    wraping_clones_[1].x = center_.x;
    wraping_clones_[1].y = center_.y - screen_size.y;
    // clone in sector 2
    wraping_clones_[2].x = center_.x - screen_size.x;
    wraping_clones_[2].y = center_.y;
  }
}