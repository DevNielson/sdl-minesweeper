#include "app.hpp"
#include <SDL3/SDL_main.h>

int main(int argc, char **argv) {
    try {
        minesweeper::App app{ "Minesweeper", 500, 500 };
        app.run();
    }
    catch (const std::exception &e) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}