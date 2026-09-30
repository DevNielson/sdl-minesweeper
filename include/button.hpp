#pragma once
#include "main.hpp"

namespace minesweeper
{
    class Button
    {
    private:
        bool m_can_cave{ true };

    private:
        SDL_Surface *m_surface;
        SDL_Texture *m_texture;
        SDL_FRect m_dstrect;
        SDL_Texture *m_flag_texture;
        
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