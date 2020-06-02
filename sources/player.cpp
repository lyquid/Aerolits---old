#include "../headers/space_objects.h"

Player::Player(const SDL_Point& screen_size): 
  kSCREEN_SIZE_({screen_size.x, screen_size.y}),
  angle_(0.f), 
  center_({screen_size.x / 2.f, screen_size.y / 2.f}),
  size_(40.f) {

  delta_ = {0.f, 0.f};
  radius_ = 0.f;
  generatePlayerShape();
  render_shape_.resize(shape_.size());
  render_shape_clones_.resize(8u);
  resizeClones();
}

void Player::move(float delta_time) {
  
  rotatePlayer();
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
  
  copyClones();
}

void Player::render(SDL_Renderer& renderer) const {
  SDL_RenderDrawLinesF(&renderer, render_shape_.data(), shape_.size());
  for (auto& clone: render_shape_clones_) {
    SDL_RenderDrawLinesF(&renderer, clone.data(), shape_.size());
  }
}

void Player::thrust(float delta_time) {
  delta_.x +=  std::sin(angle_) * 200.f * delta_time;
  delta_.y += -std::cos(angle_) * 200.f * delta_time;
}

void Player::shoot(float delta_time) {}

void Player::steerLeft(float delta_time) {
  angle_ -= 5.f * delta_time;
}

void Player::steerRight(float delta_time) {
  angle_ += 5.f * delta_time;
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

void Player::rotatePlayer() {
  for (auto i = 0u; i < shape_.size(); ++i) {
    render_shape_[i].x = (shape_[i].x * cosf(angle_) - shape_[i].y * sinf(angle_)) + center_.x;
    render_shape_[i].y = (shape_[i].x * sinf(angle_) + shape_[i].y * cosf(angle_)) + center_.y;
  }
}
