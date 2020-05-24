#ifndef AEROLITS_HEADERS_GAME_H_
#define AEROLITS_HEADERS_GAME_H_

#include <SDL.h>
#include <SDL_ttf.h>

#include "../include/cleanup.h"
#include "../include/log.h"

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
                          const std::string& file, 
                          SDL_Color color, 
                          int size, 
                          SDL_Renderer* renderer);
  void renderTexture(SDL_Texture* tex, SDL_Renderer* ren, int x, int y);

  const unsigned int kSCREEN_HEIGHT_;
  const unsigned int kSCREEN_WIDTH_;
  const SDL_Color font_color_;

  SDL_Event event_;
  SDL_Renderer* renderer_;
  SDL_Texture* score_;
  SDL_Window* main_window_;

};

#endif // AEROLITS_HEADERS_GAME_H_