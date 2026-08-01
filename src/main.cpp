#include "app.hpp"
#include <SDL3/SDL_main.h>
#include <iostream>

int main(int argc, char **argv) {
    try {
        minesweeper::App app{ "Minesweeper", 500, 500 };
        app.run();
    }
    catch (const std::exception &e) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", e.what(), nullptr);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}