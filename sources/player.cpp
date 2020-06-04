#include "../headers/space_objects.h"

Player::Player(const SDL_Point& screen_size): 
  delta_({0.f, 0.f}),
  radius_(0.f),
  angle_(0.f), 
  center_({screen_size.x / 2.f, screen_size.y / 2.f}),
  size_(40.f),
  kSCREEN_SIZE_({screen_size.x, screen_size.y}) {

  generatePlayerShape();
  render_shape_.resize(shape_.size());
  render_shape_clones_.resize(8u);
  resizeClones();
  bullets_.reserve(30);
  shooting_timer_.start();
}

void Player::move(float delta_time) {
  
  SDL_FPoint offset({0.f, 0.f});

  center_.x += delta_.x * delta_time;
  if (center_.x < 0) {
    center_.x = kSCREEN_SIZE_.x - center_.x;
    offset.x = kSCREEN_SIZE_.x - center_.x;
  } else if (center_.x > kSCREEN_SIZE_.x) {
    center_.x = center_.x - kSCREEN_SIZE_.x;
    offset.x = -(kSCREEN_SIZE_.x - center_.x);
  }
 
  center_.y += delta_.y * delta_time;
  if (center_.y < 0) {
    center_.y = kSCREEN_SIZE_.y - center_.y;
    offset.y = kSCREEN_SIZE_.y - center_.y;
  } else if (center_.y > kSCREEN_SIZE_.y) {
    center_.y = center_.y - kSCREEN_SIZE_.y;
    offset.y = -(kSCREEN_SIZE_.y - center_.y);
  }

  for (auto& point: render_shape_) {
    point.x += (delta_.x * delta_time) + offset.x;
    point.y += (delta_.y * delta_time) + offset.y;
  }
}

void Player::render(SDL_Renderer& renderer) const {
  SDL_RenderDrawLinesF(&renderer, render_shape_.data(), shape_.size());
  for (const auto& clone: render_shape_clones_) {
    SDL_RenderDrawLinesF(&renderer, clone.data(), shape_.size());
  }
  for (const auto& bullet: bullets_) {
    for (const auto& point: bullet.shape_ ) {
      SDL_RenderDrawPointF(&renderer, point.x, point.y);
    }
  }
}

void Player::thrust(float delta_time) {
  delta_.x +=  std::sin(angle_) * 200.f * delta_time;
  delta_.y += -std::cos(angle_) * 200.f * delta_time;
}

void Player::shoot(float delta_time) {
  if (shooting_timer_.getTicks() / 100 > 1) {
    bullets_.push_back({{render_shape_.front().x, render_shape_.front().y}, 
                        {50.0f * sinf(angle_), -50.0f * cosf(angle_)}});
    shooting_timer_.restart();
  }
}

void Player::steerLeft(float delta_time) {
  angle_ -= 5.f * delta_time;
}

void Player::steerRight(float delta_time) {
  angle_ += 5.f * delta_time;
}

void Player::update(float delta_time, std::vector<std::unique_ptr<Aerolite>>& aerolites) {
  rotate();
  move(delta_time);
  copyClones();
  updateBullets(delta_time, aerolites);
}

/* PRIVATE */

void Player::copyClones() {
  for (auto i = 0u; i < render_shape_clones_.size(); ++i) {
    for (auto j = 0u; j < render_shape_.size(); ++j) {
      switch (i) {
        case 0: // TOP LEFT
          render_shape_clones_[i][j].x = render_shape_[j].x - kSCREEN_SIZE_.x;
          render_shape_clones_[i][j].y = render_shape_[j].y - kSCREEN_SIZE_.y;
          break;
        case 1: // TOP MIDDLE
          render_shape_clones_[i][j].x = render_shape_[j].x;
          render_shape_clones_[i][j].y = render_shape_[j].y - kSCREEN_SIZE_.y;
          break;
        case 2: // TOP RIGHT
          render_shape_clones_[i][j].x = render_shape_[j].x + kSCREEN_SIZE_.x;
          render_shape_clones_[i][j].y = render_shape_[j].y - kSCREEN_SIZE_.y;
          break;
        case 3: // MIDDLE LEFT
          render_shape_clones_[i][j].x = render_shape_[j].x - kSCREEN_SIZE_.x;
          render_shape_clones_[i][j].y = render_shape_[j].y;
          break;
        case 4: // MIDDLE RIGHT
          render_shape_clones_[i][j].x = render_shape_[j].x + kSCREEN_SIZE_.x;
          render_shape_clones_[i][j].y = render_shape_[j].y;
          break;
        case 5: // BOTTOM LEFT
          render_shape_clones_[i][j].x = render_shape_[j].x - kSCREEN_SIZE_.x;
          render_shape_clones_[i][j].y = render_shape_[j].y + kSCREEN_SIZE_.y;
          break;
        case 6: // BOTTOM MIDDLE
          render_shape_clones_[i][j].x = render_shape_[j].x;
          render_shape_clones_[i][j].y = render_shape_[j].y + kSCREEN_SIZE_.y;
          break;
        case 7: // BOTTOM RIGHT
          render_shape_clones_[i][j].x = render_shape_[j].x + kSCREEN_SIZE_.x;
          render_shape_clones_[i][j].y = render_shape_[j].y + kSCREEN_SIZE_.y;
          break;
        default: break;
      } 
    }
  }
}

void Player::generatePlayerShape() {
  shape_.push_back({         0.f,      -size_}); // top
  shape_.push_back({-size_ / 2.f, size_ / 2.f}); // left
  shape_.push_back({-size_ / 4.f, size_ / 4.f}); // left flap
  shape_.push_back({ size_ / 4.f, size_ / 4.f}); // right flap
  shape_.push_back({ size_ / 2.f, size_ / 2.f}); // right
  shape_.push_back({         0.f,      -size_}); // top again
}

void Player::resizeClones() {
  for (auto& clone: render_shape_clones_) {
    clone.resize(render_shape_.size());
  }
}

void Player::rotate() {
  for (auto i = 0u; i < shape_.size(); ++i) {
    render_shape_[i].x = (shape_[i].x * cosf(angle_) - shape_[i].y * sinf(angle_)) + center_.x;
    render_shape_[i].y = (shape_[i].x * sinf(angle_) + shape_[i].y * cosf(angle_)) + center_.y;
  }
}

void Player::updateBullets(float delta_time, std::vector<std::unique_ptr<Aerolite>>& aerolites) {
  if (!bullets_.empty()) {
    auto bullet = bullets_.begin();
    while (bullet != bullets_.end()) {
      // update the bullet's shape
      for (auto& point: bullet->shape_) {
        point.x += 10.f * bullet->delta_.x * delta_time;
        point.y += 10.f * bullet->delta_.y * delta_time;
      }
      // check if bullet is off screen
      if (bullet->shape_.front().x < 0 || bullet->shape_.front().x >= kSCREEN_SIZE_.x
       || bullet->shape_.front().y < 0 || bullet->shape_.front().y >= kSCREEN_SIZE_.y ) {
        bullet = bullets_.erase(bullet);
      } else {
        // check collisions with aerolites
        bool collision = false;
        auto aerolite = aerolites.begin();
        while (!collision && aerolite != aerolites.end()) {
          collision = ktp::isPointInsideCircle((*aerolite)->center_.x, (*aerolite)->center_.y, (*aerolite)->radius_, bullet->shape_.front().x, bullet->shape_.front().y);
          // collision = ktp::checkCirclesCollision(0.f, bullet->shape_.front().x, bullet->shape_.front().y, (*aerolite)->radius_, (*aerolite)->shape_.front().x, (*aerolite)->shape_.front().y);
          if (collision) {
            // delete *aerolite;
            aerolite = aerolites.erase(aerolite);
          } else {
            ++aerolite;
          }
        }
        collision ? bullet = bullets_.erase(bullet) : ++bullet;
      }
    }
  }
}