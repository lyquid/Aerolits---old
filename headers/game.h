#ifndef AEROLITS_HEADERS_GAME_H_
#define AEROLITS_HEADERS_GAME_H_

#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include <SDL.h>
#include <SDL_ttf.h>

#include "../include/cleanup.h"
#include "../include/fps.h"
#include "../include/log.h"
#include "../include/resources_path.h"
#include "../include/timer.h"

#include "space_objects.h"

class Game {
 public:
  Game();
  ~Game();

 public: 
  void handleEvents();
  void handleKeyEvents(const SDL_Keycode& key);
  bool init();
  void render();
  void update();
  
  bool quit_;

 private:
  void clean();
  SDL_Texture* renderText(const std::string& message, 
                          TTF_Font* font,
                          SDL_Color color, 
                          int size, 
                          SDL_Renderer& renderer);
  void renderTexture(SDL_Texture* tex, SDL_Renderer& ren, int x, int y);

  const SDL_Color font_color_; // CONST NOTATION MISSING
  const SDL_Point kSCREEN_SIZE_;

  ktp::Timer clock_;
  SDL_Event event_;
  TTF_Font* font_;
  SDL_Window* main_window_;
  SDL_Renderer* renderer_;
  
  /* FPS related stuff */
  ktp::FPS          fps_;
  std::stringstream fps_text_;
  SDL_Texture*      fps_texture_;

  /* Asteroids stuff */
  std::vector<std::unique_ptr<Aerolite>> aerolites_;
  void generateAerolites(unsigned int number);
  void renderAerolites();
  void updateAerolites(float delta_time);

  /* Player stuff */
  Player player_;
};

#endif // AEROLITS_HEADERS_GAME_H_