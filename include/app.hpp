#pragma once
#include <SDL3/SDL.h>
#include <array>
#include <format>
#include <random>

namespace minesweeper {
    class App {
    private:
        const std::string M_TITLE;
        const int M_WIDTH;
        const int M_HEIGHT;
        static constexpr int M_AMOUNT_CELL{ 20 };
        
    private:
        SDL_Window *m_window;
        SDL_Renderer *m_renderer;
        SDL_Surface *m_surface;
        SDL_Texture *m_background_texture;
        SDL_Texture *m_mine_texture;

    private:
        std::array<std::array<int, M_AMOUNT_CELL>, M_AMOUNT_CELL> m_map;
        
    public:
        App(const std::string, const int, const int);
        ~App();
    
    private:
        void init_sdl();
        void distribution_of_mines();
        void render_background() const;
        void render_mines() const;
    
    public:
        void run();
    };
}