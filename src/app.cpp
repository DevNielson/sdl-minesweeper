#include "app.hpp"

minesweeper::App::App(const std::string title, const int width, const int height)
    : m_TITLE{ title },
      m_WIDTH{ width },
      m_HEIGHT{ height }
{
    init();
}

minesweeper::App::~App() {
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

void minesweeper::App::init() {
    if (!SDL_Init(SDL_INIT_VIDEO)) { throw std::runtime_error(std::format("Error initialize SDL: {}", SDL_GetError())); }

    m_window = SDL_CreateWindow(m_TITLE.c_str(), m_WIDTH, m_HEIGHT, 0);
    if (!m_window) { throw std::runtime_error(std::format("Error creating window: {}", SDL_GetError())); }
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
    }
}