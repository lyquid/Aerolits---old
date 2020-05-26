#ifndef AEROLITS_HEADERS_GAME_H_
#define AEROLITS_HEADERS_GAME_H_

#include <sstream>
#include <string>

#include <SDL.h>
#include <SDL_ttf.h>

#include "../include/cleanup.h"
#include "../include/fps.h"
#include "../include/log.h"
#include "../include/resources_path.h"
#include "../include/timer.h"

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
                          SDL_Renderer* renderer);
  void renderTexture(SDL_Texture* tex, SDL_Renderer* ren, int x, int y);

  const unsigned int kSCREEN_HEIGHT_;
  const unsigned int kSCREEN_WIDTH_;
  const SDL_Color font_color_; // CONST NOTATION MISSING

  SDL_Event event_;
  TTF_Font* font_;
  SDL_Window* main_window_;
  SDL_Renderer* renderer_;
  
  /* FPS related stuff */
  ktp::FPS          fps_;
  std::stringstream fps_text_;
  SDL_Texture*      fps_texture_;
};

#endif // AEROLITS_HEADERS_GAME_H_