#pragma once
#include <SDL3/SDL.h>
#include <array>
#include <format>

namespace minesweeper {
    class App {
    private:
        const std::string m_TITLE;
        const int m_WIDTH;
        const int m_HEIGHT;
        static constexpr int m_AMOUNT_CELL{ 20 };
        
    private:
        SDL_Window *m_window;
        SDL_Renderer *m_renderer;
        SDL_Surface *m_surface;
        SDL_Texture *m_texture;
        SDL_FRect m_srcrect{
            .x{},
            .y{},
            .w{ 32.0f },
            .h{ 32.0f }
        };
        SDL_FRect m_dstrect{
            .x{},
            .y{},
            .w{ static_cast<float>(m_WIDTH) / m_AMOUNT_CELL },
            .h{ static_cast<float>(m_HEIGHT) / m_AMOUNT_CELL }
        };

    private:
        std::array<std::array<int, m_AMOUNT_CELL>, m_AMOUNT_CELL> m_map;
        
    public:
        App(const std::string, const int, const int);
        ~App();
    
    private:
        void init_sdl();
        void render_map();
    
    public:
        void run();
    };
}