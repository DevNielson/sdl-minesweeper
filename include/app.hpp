#pragma once
#include <SDL3/SDL.h>
#include <format>

namespace minesweeper {
    class App {
    private:
        const std::string m_TITLE;
        const int m_WIDTH;
        const int m_HEIGHT;
        SDL_Window *m_window;
        
    public:
        App(const std::string, const int, const int);
        ~App();
    
    public:
        void init();
        void run();
    };
}