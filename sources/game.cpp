#include "../headers/game.h"

Game::Game():
  kSCREEN_HEIGHT_(600),
  kSCREEN_WIDTH_(800),
  quit_(false),
  main_window_(nullptr), 
  main_surface_(nullptr) {

  init();
}

Game::~Game() {
  clean();
}

void Game::handleEvents() {
  while (SDL_PollEvent(&event_)) {
    switch (event_.type) {
      case SDL_QUIT:
        quit_ = true;
        break;
      case SDL_KEYDOWN: 
        handleKeyEvents(event_.key.keysym.sym);
        break;
      default:
        break;
    }
  }
}

void Game::handleKeyEvents(const SDL_Keycode& key) {
  switch (key) {
    case SDLK_ESCAPE:
      quit_ = true;
      break;
    default:
      break;
  }
}

void Game::render() {
  SDL_UpdateWindowSurface(main_window_);
}

void Game::update() {
  SDL_FillRect(main_surface_, nullptr, SDL_MapRGB(main_surface_->format, 0xFF, 0xFF, 0xFF));
}

/* Private methods below */

void Game::clean() {
  ktp::cleanup(main_surface_, main_window_);
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