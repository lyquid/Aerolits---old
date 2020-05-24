#ifndef AEROLITS_INCLUDE_RESOURCES_PATH_H
#define AEROLITS_INCLUDE_RESOURCES_PATH_H

#include <string>
#include <SDL.h>

#include "log.h"

namespace ktp {

/* Original idea from Will Usher */
/* Check it here: https://github.com/Twinklebear/TwinklebearDev-Lessons */

static std::string getResourcesPath(const std::string& subDir = "") {

  constexpr auto kRESOURCES_FOLDER = "resources";

  #ifdef _WIN32
    constexpr auto kPATH_SEPARATOR = '\\';
  #else
    constexpr auto kPATH_SEPARATOR = '/';
  #endif

  static std::string base_res;

  if (base_res.empty()) {

    auto base_path = SDL_GetBasePath();

    if (base_path) {
      base_res = base_path;
      SDL_free(base_path);
    } else {
      ktp::logSDLError("SDL_GetBasePath");
      return "";
    }

    #ifdef _DEBUG
      size_t pos = base_res.rfind("Debug");
    #else
      size_t pos = base_res.rfind("Release");
    #endif

    base_res = base_res.substr(0, pos) + kRESOURCES_FOLDER + kPATH_SEPARATOR;
  }
  return subDir.empty() ? base_res : base_res + subDir + kPATH_SEPARATOR;
}

} // namespace ktp
#endif // AEROLITS_INCLUDE_RESOURCES_PATH_H