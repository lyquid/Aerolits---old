#include "../headers/game.h"

Game::Game():
  kSCREEN_HEIGHT_(600),
  kSCREEN_WIDTH_(800),
  running_(true),
  main_window_(nullptr), 
  main_surface_(nullptr) {

  init();
}

Game::~Game() {
  clean();
}

void Game::handleEvents() {}

void Game::render() {
  SDL_UpdateWindowSurface(main_window_);
  SDL_Delay(1000);
  running_ = false;
}

void Game::update() {
  SDL_FillRect(main_surface_, nullptr, SDL_MapRGB(main_surface_->format, 0xFF, 0xFF, 0xFF));
}

/* Private methods below */

void Game::clean() {
  SDL_FreeSurface(main_surface_);
	SDL_DestroyWindow(main_window_);
	SDL_Quit();
}

void Game::init() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		ktp::logSDLError("SDL_Init");
	} else {
    main_window_ = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, kSCREEN_WIDTH_, kSCREEN_HEIGHT_, SDL_WINDOW_SHOWN);
		if (main_window_ == nullptr) {
			ktp::logSDLError("SDL_CreateWindow");
		} else {
			main_surface_ = SDL_GetWindowSurface(main_window_);
    }
  }
}