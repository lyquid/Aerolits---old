#ifndef AEROLITS_INCLUDE_FPS_H_
#define AEROLITS_INCLUDE_FPS_H_

#include "timer.h"

namespace ktp {

/* Just ++FPS every SDL_RenderPresent and 
*  get your average fps calling "average()" */ 
class FPS {

 public:
  FPS(): counted_frames_(0) { fps_timer_.start(); }

  FPS& operator++() {
    ++counted_frames_;
    return *this;
  }
  FPS operator++(int) { operator++(); }

 public: 
  void addFrame(unsigned int inc = 1u) { counted_frames_ += inc; }

  float average() { return counted_frames_ / (fps_timer_.getTicks() / 1000.f); }

 private:
  unsigned long counted_frames_;
  ktp::Timer    fps_timer_;
  
};

} // namespace ktp
#endif // AEROLITS_INCLUDE_FPS_H_