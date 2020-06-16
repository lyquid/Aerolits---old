#include "../headers/game.h"

Game::Game():
  quit_(false),
  kFONT_COLOR_({0xFF, 0xFF, 0xFF, 0xFF}),
  kSCREEN_SIZE_({1024, 768}),
  event_(), 
  font_(nullptr),
  main_window_(nullptr),
  renderer_(nullptr),
  fps_texture_(nullptr),
  player_(kSCREEN_SIZE_) {

  aerolites_.reserve(50);
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

bool Game::init() {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    ktp::logSDLError("SDL_Init");
    return false;
  }

  if (TTF_Init() != 0) {
    ktp::logSDLError("TTF_Init");
    SDL_Quit();
    return false;
  }

  main_window_ = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, kSCREEN_SIZE_.x, kSCREEN_SIZE_.y, SDL_WINDOW_SHOWN);
  if (main_window_ == nullptr) {
	  ktp::logSDLError("SDL_CreateWindow");
    TTF_Quit();
    SDL_Quit();
    return false;
	}

  renderer_ = SDL_CreateRenderer(main_window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer_ == nullptr) {
    ktp::logSDLError("SDL_CreateRenderer");
    ktp::cleanup(main_window_);
    TTF_Quit();
    SDL_Quit();
    return false;
  }

  // font_ = TTF_OpenFont((ktp::getResourcesPath() + "Future n0t Found.ttf").c_str(), 32); // need to fix this
  font_ = TTF_OpenFont("./resources/Future n0t Found.ttf", 18u);
  if (font_ == nullptr) {
    ktp::logSDLError("TTF_OpenFont");
    ktp::cleanup(renderer_, main_window_);
    TTF_Quit();
    SDL_Quit();
    return false;
  }

  generateAerolites(6);
  
  return true;
}

void Game::render() {
  SDL_SetRenderDrawColor(renderer_, 0x00, 0x00, 0x00, 0x00);
  SDL_RenderClear(renderer_);

  renderTexture(fps_texture_, *renderer_, 0u, 0u);
  renderAerolites();
  player_.render(*renderer_);

  SDL_RenderPresent(renderer_);
  ++fps_;
}

void Game::update() {
  /* FPS */
  fps_text_.str(std::string());
  fps_text_ << fps_.average();
  ktp::cleanup(fps_texture_); // <-- is this really necessary? seems to...
  fps_texture_ = renderText(fps_text_.str(), font_, kFONT_COLOR_, 8, *renderer_); // size not working

  const float delta_time = clock_.restart() / 1000.f;

  /* Aerolites */
  Aerolite::updateAerolites(delta_time, kSCREEN_SIZE_, aerolites_);

  /* Player */ 
  if (player_.isAlive()) {
    checkKeyStates(delta_time);
    player_.update(delta_time, aerolites_);  
  } else {
    player_.reset();
  }
 }

/* PRIVATE */

void Game::checkKeyStates(float delta_time) {
  const Uint8* state = SDL_GetKeyboardState(nullptr);
  if (state[SDL_SCANCODE_W] || state[SDL_SCANCODE_UP]){
    player_.thrust(delta_time);
  }
  if (state[SDL_SCANCODE_A] || state[SDL_SCANCODE_LEFT]){
    player_.steerLeft(delta_time);
  }
  if (state[SDL_SCANCODE_D] || state[SDL_SCANCODE_RIGHT]){
    player_.steerRight(delta_time);
  }
  if (state[SDL_SCANCODE_SPACE]){
    player_.shoot(delta_time);
  }
}

void Game::clean() {
  ktp::cleanup(fps_texture_, renderer_, main_window_, font_);
  TTF_Quit();
	SDL_Quit();
}

void Game::generateAerolites(unsigned int number) {
  unsigned int count = 0u;
  unsigned int too_many = 0u;
  Aerolite* aero;
  
  do {
    bool bad_place = false;
    aero = new Aerolite(kSCREEN_SIZE_);
    for (auto i = 0u; i < aerolites_.size() && !bad_place; ++i) {
      if (ktp::checkCircleAABBCollision(aero->center_, aero->radius_, aerolites_[i]->center_, aerolites_[i]->radius_)) {
        bad_place = true;
      }
    }
    if (!bad_place) {
      aerolites_.push_back(std::unique_ptr<Aerolite>(aero));
      ++count;
    } else {
      delete aero;
    }
    if (++too_many > (kSCREEN_SIZE_.x + kSCREEN_SIZE_.y) / 50u) break;
  } while (count < number);

  // aerolites_.push_back(std::unique_ptr<Aerolite>(new Aerolite(100, 140,  -100, 0, 200)));
  // aerolites_.push_back(std::unique_ptr<Aerolite>(new Aerolite(600, 140,   -10, 0, 200)));
} 

void Game::renderAerolites() {
  for (const auto& aerolite: aerolites_ ) {
    aerolite->render(*renderer_);
  } 
}

/** 
* Original idea from Will Usher 
* Check it here: https://github.com/Twinklebear/TwinklebearDev-Lessons 
*
* Render the message we want to display to a texture for drawing.
* @param message The message we want to display.
* @param font The font we want to use to render the text.
* @param color The color we want the text to be.
* @param size The size we want the font to be.
* @param renderer The renderer to load the texture in.
*/
SDL_Texture* Game::renderText(const std::string& message, TTF_Font* font,
	                            SDL_Color color, int size, SDL_Renderer& renderer) {

  SDL_Surface* surface = TTF_RenderText_Blended(font, message.c_str(), color);
  if (surface == nullptr) {
    ktp::logSDLError("TTF_RenderText_Blended");
    return nullptr;
  }

  SDL_Texture* texture = SDL_CreateTextureFromSurface(&renderer, surface);
  if (texture == nullptr) {
    ktp::logSDLError("SDL_CreateTextureFromSurface");
  }

  ktp::cleanup(surface);
  return texture;
}

/**
* Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
* the texture's width and height.
* @param tex The source texture we want to draw.
* @param ren The renderer we want to draw to.
* @param x The x coordinate to draw to.
* @param y The y coordinate to draw to.
*/
void Game::renderTexture(SDL_Texture* tex, SDL_Renderer& renderer, int x, int y){
	// Setup the destination rectangle to be at the position we want
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	// Query the texture to get its width and height to use
	SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
	SDL_RenderCopy(&renderer, tex, NULL, &dst);
}
