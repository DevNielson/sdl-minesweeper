#include "app.hpp"

minesweeper::App::App(const std::string title, const int width, const int height)
    : m_TITLE{ title },
      m_WIDTH{ width },
      m_HEIGHT{ height }
{
    init_sdl();
    m_map.fill({ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 });
}

minesweeper::App::~App() {
    SDL_DestroyTexture(m_texture);
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

void minesweeper::App::init_sdl() {
    if (!SDL_Init(SDL_INIT_VIDEO)) { throw std::runtime_error(std::format("Error initialize SDL: {}", SDL_GetError())); }

    m_window = SDL_CreateWindow(m_TITLE.c_str(), m_WIDTH, m_HEIGHT, 0);
    if (!m_window) { throw std::runtime_error(std::format("Error creating window: {}", SDL_GetError())); }

    m_renderer = SDL_CreateRenderer(m_window, nullptr);
    if (!m_renderer) { throw std::runtime_error(std::format("Error creating renderer: {}", SDL_GetError())); }
    
    m_surface = SDL_LoadPNG("../assets/map-1.png");
    if (!m_surface) { throw std::runtime_error(std::format("Error loading surface: {}", SDL_GetError())); }
    
    m_texture = SDL_CreateTextureFromSurface(m_renderer, m_surface);
    SDL_DestroySurface(m_surface);
    if (!m_texture) { throw std::runtime_error(std::format("Error creating texture: {}", SDL_GetError())); }
    
    if (!SDL_SetTextureScaleMode(m_texture, SDL_SCALEMODE_NEAREST)) {
        throw std::runtime_error(std::format("Error setting texture scale mode: {}", SDL_GetError()));
    }
}

void minesweeper::App::render_map() {
    for (int i{}; i < m_AMOUNT_CELL; ++i) {
        for (int j{}; j < m_AMOUNT_CELL; ++j) {
            m_dstrect.x = i * static_cast<float>(m_WIDTH) / m_AMOUNT_CELL;
            m_dstrect.y = j * static_cast<float>(m_HEIGHT) / m_AMOUNT_CELL;
            if (!SDL_RenderTexture(m_renderer, m_texture, &m_srcrect, &m_dstrect)) {
                throw std::format("Error rendering texture: {}", SDL_GetError());
            }
        }
    }
}

void minesweeper::App::run() {
    SDL_Event test_event{};
    bool is_running{ true };
    while (is_running) {
        while (SDL_PollEvent(&test_event)) {
            switch (test_event.type) {
                case SDL_EVENT_QUIT:
                    is_running = false;
                    break;
                default:
                    break;
            }
        }
        SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
        SDL_RenderClear(m_renderer);
        render_map();
        SDL_RenderPresent(m_renderer);
    }
}