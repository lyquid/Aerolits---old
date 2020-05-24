#include "../headers/game.h"

Game::Game():
  kSCREEN_HEIGHT_(600),
  kSCREEN_WIDTH_(800),
  font_color_({255, 255, 255, 255}),
  quit_(false),
  main_window_(nullptr),
  renderer_(nullptr),
  score_(nullptr) {}

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

  score_ = renderText("holamanola", "./resources/Future n0t Found.ttf", font_color_, 64, renderer_);
  if (score_ == nullptr) {
    ktp::cleanup(renderer_, main_window_);
    TTF_Quit();
    SDL_Quit();
    return false;
  }
  return true;
}

void Game::render() {
  int width, height;
  SDL_QueryTexture(score_, NULL, NULL, &width, &height);
  int x = kSCREEN_WIDTH_ / 2 - width / 2;
  int y = kSCREEN_HEIGHT_ / 2 - height / 2;

  SDL_RenderClear(renderer_);
  renderTexture(score_, renderer_, x, y);
  SDL_RenderPresent(renderer_);
}

void Game::update() { }

/* Private methods below */

void Game::clean() {
  ktp::cleanup(score_, renderer_, main_window_);
  TTF_Quit();
	SDL_Quit();
}

/* Original idea from Will Usher */
/* Check it here: https://github.com/Twinklebear/TwinklebearDev-Lessons */
/**
* Render the message we want to display to a texture for drawing.
* @param message The message we want to display.
* @param fontFile The font we want to use to render the text.
* @param color The color we want the text to be.
* @param fontSize The size we want the font to be.
* @param renderer The renderer to load the texture in.
* @return An SDL_Texture containing the rendered message, or nullptr if something went wrong.
*/
SDL_Texture* Game::renderText(const std::string& message, const std::string& file,
	                      SDL_Color color, int size, SDL_Renderer* renderer) {
    
  TTF_Font* font = TTF_OpenFont(file.c_str(), size);
  if (font == nullptr) {
    ktp::logSDLError("TTF_OpenFont");
    return nullptr;
  }

  SDL_Surface *surface = TTF_RenderText_Blended(font, message.c_str(), color);
  if (surface == nullptr) {
    ktp::logSDLError("TTF_RenderText_Blended");
    TTF_CloseFont(font);
    return nullptr;
  }

  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
  if (texture == nullptr) {
    ktp::logSDLError("SDL_CreateTextureFromSurface");
  }

  ktp::cleanup(surface, font);
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