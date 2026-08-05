#pragma once
#include "main.hpp"

namespace minesweeper {
    class Button {
    private:
        SDL_Surface *m_surface;
        SDL_Texture *m_texture;
        SDL_FRect m_dstrect;
        
    public:
        Button() = default;
        Button(SDL_Renderer *, const std::string, const SDL_FRect);
        ~Button();
        
    public:
        bool update();
        
    public:
        void render_button(SDL_Renderer *) const;
    };
}