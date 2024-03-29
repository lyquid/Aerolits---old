/* Original idea from Will Usher */
/* Check it here: https://github.com/Twinklebear/TwinklebearDev-Lessons */

#ifndef AEROLITS_INCLUDE_CLEANUP_H_
#define AEROLITS_INCLUDE_CLEANUP_H_

#include <utility>
#include "SDL.h"

namespace ktp {

/*
 * Recurse through the list of arguments to clean up, cleaning up
 * the first one in the list each iteration.
 */
template<typename T, typename... Args>
void cleanup(T *t, Args&&... args){
	// Cleanup the first item in the list
	cleanup(t);
	// Clean up the remaining arguments
	cleanup(std::forward<Args>(args)...);
}

/*
 * These specializations serve to free the passed argument and also provide the
 * base cases for the recursive call above, eg. when args is only a single
 * item one of the specializations below will be called by
 * cleanup(std::forward<Args>(args)...), ending the recursion
 */
template<>inline void cleanup<SDL_Window>(SDL_Window* win){
	if (!win) return;
	SDL_DestroyWindow(win);
}

template<>inline void cleanup<SDL_Renderer>(SDL_Renderer* ren){
	if (!ren) return;
	SDL_DestroyRenderer(ren);
}

template<>inline void cleanup<SDL_Texture>(SDL_Texture* tex){
	if (!tex) return;
	SDL_DestroyTexture(tex);
}

template<>inline void cleanup<SDL_Surface>(SDL_Surface* surf){
	if (!surf) return;
	SDL_FreeSurface(surf);
}

template<>inline void cleanup<TTF_Font>(TTF_Font* font){
	if (!font) return;
	TTF_CloseFont(font);
}

} // namespace ktp

#endif // AEROLITS_INCLUDE_CLEANUP_H_