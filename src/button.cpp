#include "button.hpp"

minesweeper::Button::Button(SDL_Renderer *renderer, const std::string filepath, const SDL_FRect rect)
    : m_dstrect{ rect }
{
    m_surface = SDL_LoadPNG(filepath.c_str());
    if (!m_surface) { throw std::runtime_error(std::format("Error setting surface: {}", SDL_GetError())); }
    
    m_texture = SDL_CreateTextureFromSurface(renderer, m_surface);
    SDL_DestroySurface(m_surface);
    if (!m_texture) { throw std::runtime_error(std::format("Error creating texture: {}", SDL_GetError())); }
    
    if(!SDL_SetTextureScaleMode(m_texture, SDL_SCALEMODE_NEAREST)) {
        throw std::runtime_error(std::format("Error setting texture scale mode: {}", SDL_GetError()));
    }
}

minesweeper::Button::~Button() {
    // SDL_DestroyTexture(m_texture);
}

bool minesweeper::Button::update() {
    float x{}, y{};
    if ((SDL_GetMouseState(&x, &y) == SDL_BUTTON_LEFT) &&
        ((x > m_dstrect.x) && (x < m_dstrect.x + m_dstrect.w)) &&
        ((y > m_dstrect.y) && (y < m_dstrect.y + m_dstrect.w))
    ) { return false; }
    return true;
}

void minesweeper::Button::render_button(SDL_Renderer *renderer) const {
    if (!SDL_RenderTexture(renderer, m_texture, nullptr, &m_dstrect)) {
        throw std::runtime_error(std::format("Error rendering texture {}", SDL_GetError()));
    }
}