#ifndef AEROLITS_HEADERS_GAME_H_
#define AEROLITS_HEADERS_GAME_H_

#include <SDL.h>

#include "../include/cleanup.h"
#include "../include/log.h"

class Game {
 public:
  Game();
  ~Game();

 public: 
  void handleEvents();
  void handleKeyEvents(const SDL_Keycode& key);
  void render();
  void update();
  
  bool quit_;


 private:
  void clean();
  void init();

  const unsigned int kSCREEN_HEIGHT_;
  const unsigned int kSCREEN_WIDTH_;

  SDL_Event event_;
  SDL_Surface* main_surface_;
  SDL_Window* main_window_;

};

#endif // AEROLITS_HEADERS_GAME_H_