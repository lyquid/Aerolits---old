#include "../headers/game.h"

Game::Game():
  quit_(false),
  kSCREEN_HEIGHT_(768),
  kSCREEN_WIDTH_(1024),
  font_color_({255, 255, 255, 255}),
  font_(nullptr),
  main_window_(nullptr),
  renderer_(nullptr),
  fps_texture_(nullptr) {}

Game::~Game() { clean(); }

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

  main_window_ = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, kSCREEN_WIDTH_, kSCREEN_HEIGHT_, SDL_WINDOW_SHOWN);
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
  return true;
}

void Game::render() {
  SDL_RenderClear(renderer_);
  renderTexture(fps_texture_, renderer_, 0u, 0u);
  SDL_RenderPresent(renderer_);
  ++fps_;
}

void Game::update() {
  fps_text_.str(std::string());
  fps_text_ << fps_.average();

  ktp::cleanup(fps_texture_); // <-- is this really necessary? seems to...
  fps_texture_ = renderText(fps_text_.str(), font_, font_color_, 8, renderer_); // size not working
 }

/* Private methods below */

void Game::clean() {
  ktp::cleanup(fps_texture_, renderer_, main_window_, font_);
  TTF_Quit();
	SDL_Quit();
}

/* Original idea from Will Usher */
/* Check it here: https://github.com/Twinklebear/TwinklebearDev-Lessons */
/**
* Render the message we want to display to a texture for drawing.
* @param message The message we want to display.
* @param font The font we want to use to render the text.
* @param color The color we want the text to be.
* @param size The size we want the font to be.
* @param renderer The renderer to load the texture in.
*/
SDL_Texture* Game::renderText(const std::string& message, TTF_Font* font,
	                            SDL_Color color, int size, SDL_Renderer* renderer) {
    
  SDL_Surface* surface = TTF_RenderText_Blended(font, message.c_str(), color);
  if (surface == nullptr) {
    ktp::logSDLError("TTF_RenderText_Blended");
    return nullptr;
  }

  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
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
void Game::renderTexture(SDL_Texture* tex, SDL_Renderer* ren, int x, int y){
	// Setup the destination rectangle to be at the position we want
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	// Query the texture to get its width and height to use
	SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
	SDL_RenderCopy(ren, tex, NULL, &dst);
}