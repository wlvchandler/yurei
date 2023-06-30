#include <j16/j16.h>

#include <SDL.h>
#include <j16/runtime.h>

#if defined(_WIN32) || defined(_WIN64)
int SDL_main(int argc, char *argv[]) {
#else
int main() {
#endif
    return 0;
}