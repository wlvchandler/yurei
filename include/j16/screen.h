#include <j16/j16.h>

#include <chrono>
#include <thread>
#include <vector>
#include <cstdint>

constexpr SCREEN_WIDTH = 240;
constexpr SCREEN_HEIGHT = 240;
constexpr SCREEN_SCALE = 2;

class TFTScreen {
public:
	TFTScreen();
	~TFTScreen();

	void clear();
	void update();
	SDL_Renderer* getRenderer();
};
