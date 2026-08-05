#pragma once
#include "button.hpp"
#include <SDL3_ttf/SDL_ttf.h>
#include <array>
#include <random>
#include <string>
#include <vector>

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
        TTF_Font *m_font;
        std::array<SDL_Texture *, 8> m_numbers_textures;

    private:
        std::array<std::array<int, M_AMOUNT_CELL>, M_AMOUNT_CELL> m_map;
        std::vector<Button> m_buttons;
        
    public:
        App(const std::string, const int, const int);
        ~App();
    
    private:
        void init_sdl();
        void distribution_of_mines();
        void distribution_of_numbers();
        void distribution_of_buttons();
        
    private:
        void render_background() const;
        void render_mines() const;
        void render_numbers() const;
    
    public:
        void run();
    };
}