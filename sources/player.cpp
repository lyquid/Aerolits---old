#include "../headers/space_objects.h"

Player::Player(const SDL_Point& screen_size) {
  angle_ = 0.f;
  delta_ = {0.f, 0.f};
  size_ = 40u,
  radius_ = 0.f;
  center_ = {screen_size.x / 2.f, screen_size.y / 2.f};
  generatePlayerShape(center_);
}

void Player::render(SDL_Renderer& renderer) const {
  for (const auto& point: shape_) {
    SDL_RenderDrawPointF(&renderer, point.x, point.y);
  }
}

void Player::generatePlayerShape(const SDL_FPoint& where) {
  shape_.push_back({where.x                , where.y - (size_ / 2.f)});
  shape_.push_back({where.x - (size_ / 3.f), where.y + (size_ / 2.f)});
  shape_.push_back({where.x + (size_ / 3.f), where.y + (size_ / 2.f)});
  shape_.push_back(where);
}