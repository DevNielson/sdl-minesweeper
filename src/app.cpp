#include "app.hpp"
#include <print>

minesweeper::App::App(const std::string title, const int width, const int height)
    : M_TITLE{ title },
      M_WIDTH{ width },
      M_HEIGHT{ height }
{
    init_sdl();
    distribution_of_mines();
    distribution_of_numbers();
    
    // For tests!!!
    for (auto a : m_map) {
        for (auto b : a) {
            std::print("{} ", b);
        }
        std::println();
    }
}

minesweeper::App::~App() {
    SDL_DestroyTexture(m_mine_texture);
    SDL_DestroyTexture(m_background_texture);
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

void minesweeper::App::init_sdl() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        throw std::runtime_error(std::format("Error initialize SDL: {}", SDL_GetError()));
    }

    m_window = SDL_CreateWindow(M_TITLE.c_str(), M_WIDTH, M_HEIGHT, 0);
    if (!m_window) { throw std::runtime_error(std::format("Error creating window: {}", SDL_GetError())); }

    m_renderer = SDL_CreateRenderer(m_window, nullptr);
    if (!m_renderer) { throw std::runtime_error(std::format("Error creating renderer: {}", SDL_GetError())); }
    
    m_surface = SDL_LoadPNG("../assets/map-1.png");
    if (!m_surface) { throw std::runtime_error(std::format("Error loading surface map-1.png: {}", SDL_GetError())); }
    
    m_background_texture = SDL_CreateTextureFromSurface(m_renderer, m_surface);
    SDL_DestroySurface(m_surface);
    if (!m_background_texture) {
        throw std::runtime_error(std::format("Error creating background texture: {}", SDL_GetError()));
    }
    
    if (!SDL_SetTextureScaleMode(m_background_texture, SDL_SCALEMODE_NEAREST)) {
        throw std::runtime_error(std::format("Error setting background texture scale mode: {}", SDL_GetError()));
    }
    
    m_surface = SDL_LoadPNG("../assets/map-2.png");
    if (!m_surface) { throw std::runtime_error(std::format("Error loading surface map-2.png: {}", SDL_GetError())); }
    
    m_mine_texture = SDL_CreateTextureFromSurface(m_renderer, m_surface);
    SDL_DestroySurface(m_surface);
    if (!m_mine_texture) { throw std::runtime_error(std::format("Error creating mine texture: {}", SDL_GetError())); }
    
    if (!SDL_SetTextureScaleMode(m_background_texture, SDL_SCALEMODE_NEAREST)) {
        throw std::runtime_error(std::format("Error setting mine texture scale mode: {}", SDL_GetError()));
    }
}

void minesweeper::App::distribution_of_mines() {
    std::uniform_int_distribution<int> distribution(0, 5);
    std::random_device random;

    for (auto &line : m_map) {
        for (auto &cell : line) {
            cell = (distribution(random) == 0) ? (-1) : (0);
        }
    }
}

void minesweeper::App::distribution_of_numbers() {
    for (int i{}; i < M_AMOUNT_CELL; ++i) {
        for (int j{}; j < M_AMOUNT_CELL; ++j) {
            int counter{};
            
            if (m_map.at(i).at(j) == -1) { continue; }

            if (i - 1 >= 0) {
                if (m_map.at(i - 1).at(j) == -1) { ++counter; }
            }
            if (i + 1 < M_AMOUNT_CELL) {
                if (m_map.at(i + 1).at(j) == -1) { ++counter; }
            }
            if (j - 1 >= 0) {
                if (m_map.at(i).at(j - 1) == -1) { ++counter; }
            }
            if (j + 1 < M_AMOUNT_CELL) {
                if (m_map.at(i).at(j + 1) == -1) { ++counter; }
            }
            
            if ((i - 1 >= 0) && (j - 1 >= 0)) {
                if (m_map.at(i - 1).at(j - 1) == -1) { ++counter; }
            }
            if ((i - 1 >= 0) && (j + 1 < M_AMOUNT_CELL)) {
                if (m_map.at(i - 1).at(j + 1) == -1) { ++counter; }
            }
            if ((i + 1 < M_AMOUNT_CELL) && (j + 1 < M_AMOUNT_CELL)) {
                if (m_map.at(i + 1).at(j + 1) == -1) { ++counter; }
            }
            if ((i + 1 < M_AMOUNT_CELL) && (j - 1 >= 0)) {
                if (m_map.at(i + 1).at(j - 1) == -1) { ++counter; }
            }
            
            m_map.at(i).at(j) = counter;
        }
    }
}

void minesweeper::App::render_background() const {
    for (int i{}; i < M_AMOUNT_CELL; ++i) {
        for (int j{}; j < M_AMOUNT_CELL; ++j) {
            SDL_FRect dstrect{
                .x{ j * static_cast<float>(M_WIDTH) / M_AMOUNT_CELL },
                .y{ i * static_cast<float>(M_HEIGHT) / M_AMOUNT_CELL },
                .w{ static_cast<float>(M_WIDTH) / M_AMOUNT_CELL },
                .h{ static_cast<float>(M_HEIGHT) / M_AMOUNT_CELL }
            };

            if (!SDL_RenderTexture(m_renderer, m_background_texture, nullptr, &dstrect)) {
                throw std::format("Error rendering background texture: {}", SDL_GetError());
            }
        }
    }
}

void minesweeper::App::render_mines() const {
    for (int i{}; i < M_AMOUNT_CELL; ++i) {
        for (int j{}; j < M_AMOUNT_CELL; ++j) {
            if (m_map.at(i).at(j) != -1) { continue; }

            SDL_FRect dstrect{
                .x{ j * static_cast<float>(M_WIDTH) / M_AMOUNT_CELL },
                .y{ i * static_cast<float>(M_HEIGHT) / M_AMOUNT_CELL },
                .w{ static_cast<float>(M_WIDTH) / M_AMOUNT_CELL },
                .h{ static_cast<float>(M_HEIGHT) / M_AMOUNT_CELL }
            };

            if (!SDL_RenderTexture(m_renderer, m_mine_texture, nullptr, &dstrect)) {
                throw std::format("Error rendering mine texture: {}", SDL_GetError());
            }
        }
    }
}

void minesweeper::App::run() {
    bool is_running{ true };
    while (is_running) {
        SDL_Event test_event{};
        while (SDL_PollEvent(&test_event)) {
            switch (test_event.type) {
                case SDL_EVENT_QUIT:
                    is_running = false;
                    break;
                default:
                    break;
            }
        }
        SDL_RenderClear(m_renderer);
        render_background();
        render_mines();
        SDL_RenderPresent(m_renderer);
    }
}