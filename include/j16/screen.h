#include <SDL.h>
#include <chrono>
#include <cstdint>
#include <thread>
#include <vector>

constexpr uint8_t SCREEN_WIDTH = 240;
constexpr uint8_t SCREEN_HEIGHT = 240;
constexpr uint8_t SCREEN_SCALE = 2;

class TFTScreen {
  public:
    TFTScreen();
    ~TFTScreen();

    void clear();
    void update();
    SDL_Renderer* getRenderer();
};