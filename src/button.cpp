#include "button.hpp"

minesweeper::Button::Button(SDL_Renderer *renderer, const std::string filepath, const SDL_FRect rect)
    : m_dstrect{ rect }
{
    m_surface = SDL_LoadPNG(filepath.c_str());
    if (!m_surface) { throw std::runtime_error(std::format("Error setting surface: {}", SDL_GetError())); }
    
    m_texture = SDL_CreateTextureFromSurface(renderer, m_surface);
    SDL_DestroySurface(m_surface);
    if (!m_texture) { throw std::runtime_error(std::format("Error creating texture: {}", SDL_GetError())); }
    
    if(!SDL_SetTextureScaleMode(m_texture, SDL_SCALEMODE_PIXELART))
    { throw std::runtime_error(std::format("Error setting texture scale mode: {}", SDL_GetError())); }

    m_surface = SDL_LoadPNG("../assets/map-4.png");
    if (!m_surface) { throw std::runtime_error(std::format("Error setting surface: {}", SDL_GetError())); }
    
    m_flag_texture = SDL_CreateTextureFromSurface(renderer, m_surface);
    SDL_DestroySurface(m_surface);
    if (!m_texture) { throw std::runtime_error(std::format("Error creating flag texture: {}", SDL_GetError())); }
    
    if(!SDL_SetTextureScaleMode(m_flag_texture, SDL_SCALEMODE_PIXELART))
    { throw std::runtime_error(std::format("Error setting flag texture scale mode: {}", SDL_GetError())); }
}

minesweeper::Button::~Button() { /* SDL_DestroyTexture(m_texture);*/ }

bool minesweeper::Button::update()
{
    float x{}, y{};
    const SDL_MouseButtonFlags MOUSE_BUTTON_FLAG{ SDL_GetMouseState(&x, &y) };

    if ((MOUSE_BUTTON_FLAG == SDL_BUTTON_X1)
        && ((x > m_dstrect.x) && (x < m_dstrect.x + m_dstrect.w))
        && ((y > m_dstrect.y) && (y < m_dstrect.y + m_dstrect.w)))
    { m_can_cave = !m_can_cave; }

    if ((MOUSE_BUTTON_FLAG == SDL_BUTTON_LEFT)
        && ((x > m_dstrect.x) && (x < m_dstrect.x + m_dstrect.w))
        && ((y > m_dstrect.y) && (y < m_dstrect.y + m_dstrect.w))
        && m_can_cave)
    { return false; }

    return true;
}

void minesweeper::Button::render_button(SDL_Renderer *renderer) const
{
    if (!SDL_RenderTexture(renderer, m_texture, nullptr, &m_dstrect))
    { throw std::runtime_error(std::format("Error rendering texture {}", SDL_GetError())); }
    
    if (!m_can_cave)
    {
        SDL_FRect dstrect{
            .x{ m_dstrect.x },
            .y{ m_dstrect.y },
            .w{ CELL_SIZE },
            .h{ CELL_SIZE }
        };

        if (!SDL_RenderTexture(renderer, m_flag_texture, nullptr, &m_dstrect))
        { throw std::runtime_error(std::format("Error rendering flag texture {}", SDL_GetError())); }
    }
}