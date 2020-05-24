#include <random>
#include <ctime>

#include "headers/game.h"

int main(int argc, char *argv[]) {

  std::srand(static_cast<unsigned int>(std::time(nullptr)));

  Game* game = new Game();

  if (game->init()) {

    while (!game->quit_) {
      game->handleEvents();
      game->update();
      game->render();
    }
    
    delete game;
    return 0;

  } else {
    // something went wrong
    delete game;
    return 1;
  }
}