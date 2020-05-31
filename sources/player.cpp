#include "../headers/space_objects.h"

Player::Player(const SDL_Point& screen_size) {
  angle_ = 0.f;
  delta_ = {0.f, 0.f};
  size_ = 40.f,
  radius_ = 0.f;
  center_ = {screen_size.x / 2.f, screen_size.y / 2.f};
  generatePlayerShape();
  render_shape_.resize(shape_.size());
}

void Player::move(float delta_time) {
  rotatePlayer();
  for (auto& point: render_shape_) {
    point.x += delta_.x * delta_time;
    point.y += delta_.y * delta_time;
  }
  center_.x += delta_.x * delta_time;
  center_.y += delta_.y * delta_time;
}

void Player::render(SDL_Renderer& renderer) const {
  const SDL_FPoint* points = render_shape_.data();
  SDL_RenderDrawLinesF(&renderer, points, shape_.size());
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

void Player::generatePlayerShape() {
  shape_.push_back({         0.f,      -size_}); // top
  shape_.push_back({-size_ / 2.f, size_ / 2.f}); // left
  shape_.push_back({-size_ / 4.f, size_ / 4.f}); // left flap
  shape_.push_back({ size_ / 4.f, size_ / 4.f}); // right flap
  shape_.push_back({ size_ / 2.f, size_ / 2.f}); // right
  shape_.push_back({         0.f,      -size_}); // top again
}

void Player::rotatePlayer() {
  for (auto i = 0u; i < shape_.size(); ++i) {
    render_shape_[i].x = (shape_[i].x * std::cosf(angle_) - shape_[i].y * std::sinf(angle_)) + center_.x;
    render_shape_[i].y = (shape_[i].x * std::sinf(angle_) + shape_[i].y * std::cosf(angle_)) + center_.y;
  }
}
