#include "app.hpp"

minesweeper::App::App(const std::string title, const int row_number_cells, const int column_number_cells)
    : M_TITLE{ title },
      M_ROW_NUMBER_CELLS{ row_number_cells },
      M_COLUMN_NUMBER_CELLS{ column_number_cells }
{
    init_sdl();
    distribution_of_mines();
    distribution_of_numbers();
    distribution_of_buttons();
}

minesweeper::App::~App() {
    for (SDL_Texture *number_texture : m_numbers_textures) { SDL_DestroyTexture(number_texture); }
    TTF_Quit();
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

    m_window = SDL_CreateWindow(M_TITLE.c_str(), M_COLUMN_NUMBER_CELLS * CELL_SIZE, M_ROW_NUMBER_CELLS * CELL_SIZE, 0);
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

    if (!TTF_Init()) { throw std::runtime_error(std::format("Error initialize TTF: {}", SDL_GetError())); }
    
    m_font = TTF_OpenFont("../font/Vaticanus-G3yVG.ttf", 32.0f);
    if (!m_font) { throw std::runtime_error(std::format("Error open font: {}", SDL_GetError())); }
    
    for (int i{}; i < 8; ++i) {
        m_surface = TTF_RenderText_Blended(m_font, std::to_string(i + 1).c_str(), 0, SDL_Color{ 0, 0, 255, 255 });
        if (!m_surface) { throw std::runtime_error(std::format("Error loading text surface: {}", SDL_GetError())); }

        m_numbers_textures.at(i) = SDL_CreateTextureFromSurface(m_renderer, m_surface);
        if (!m_mine_texture) { throw std::runtime_error(std::format("Error creating numbers texture: {}", SDL_GetError())); }

        if (!SDL_SetTextureScaleMode(m_numbers_textures.at(i), SDL_SCALEMODE_NEAREST)) {
            throw std::runtime_error(std::format("Error setting numbers texture scale mode: {}", SDL_GetError()));
        }
    }
    SDL_DestroySurface(m_surface);
}

void minesweeper::App::distribution_of_mines() {
    std::uniform_int_distribution<int> distribution(0, 5);
    std::random_device random;

    for (int i{}; i < M_ROW_NUMBER_CELLS; ++i) {
        std::vector<std::int8_t> row;
        for (int j{}; j < M_COLUMN_NUMBER_CELLS; ++j) {
            row.push_back((distribution(random) == 0) ? (-1) : (0));
        }
        m_map.push_back(row);
    }
}

void minesweeper::App::distribution_of_numbers() {
    for (int i{}; i < M_ROW_NUMBER_CELLS; ++i) {
        for (int j{}; j < M_COLUMN_NUMBER_CELLS; ++j) {
            int counter{};
            
            if (m_map.at(i).at(j) == -1) { continue; }

            if (i - 1 >= 0) {
                if (m_map.at(i - 1).at(j) == -1) { ++counter; }
            }
            if (i + 1 < M_ROW_NUMBER_CELLS) {
                if (m_map.at(i + 1).at(j) == -1) { ++counter; }
            }
            if (j - 1 >= 0) {
                if (m_map.at(i).at(j - 1) == -1) { ++counter; }
            }
            if (j + 1 < M_COLUMN_NUMBER_CELLS) {
                if (m_map.at(i).at(j + 1) == -1) { ++counter; }
            }
            
            if ((i - 1 >= 0) && (j - 1 >= 0)) {
                if (m_map.at(i - 1).at(j - 1) == -1) { ++counter; }
            }
            if ((i - 1 >= 0) && (j + 1 < M_COLUMN_NUMBER_CELLS)) {
                if (m_map.at(i - 1).at(j + 1) == -1) { ++counter; }
            }
            if ((i + 1 < M_ROW_NUMBER_CELLS) && (j + 1 < M_COLUMN_NUMBER_CELLS)) {
                if (m_map.at(i + 1).at(j + 1) == -1) { ++counter; }
            }
            if ((i + 1 < M_ROW_NUMBER_CELLS) && (j - 1 >= 0)) {
                if (m_map.at(i + 1).at(j - 1) == -1) { ++counter; }
            }
            
            m_map.at(i).at(j) = counter;
        }
    }
}

void minesweeper::App::distribution_of_buttons() {
    for (int i{}; i < M_ROW_NUMBER_CELLS; ++i) {
        for (int j{}; j < M_COLUMN_NUMBER_CELLS; ++j) {
            m_buttons.push_back({
                m_renderer,
                "../assets/map-3.png",
                SDL_FRect{
                    j * CELL_SIZE,
                    i * CELL_SIZE,
                    CELL_SIZE,
                    CELL_SIZE
            }});
        }
    }
}

void minesweeper::App::render_background() const {
    for (int i{}; i < M_ROW_NUMBER_CELLS; ++i) {
        for (int j{}; j < M_COLUMN_NUMBER_CELLS; ++j) {
            SDL_FRect dstrect{
                .x{ j * CELL_SIZE },
                .y{ i * CELL_SIZE },
                .w{ CELL_SIZE },
                .h{ CELL_SIZE }
            };

            if (!SDL_RenderTexture(m_renderer, m_background_texture, nullptr, &dstrect)) {
                throw std::format("Error rendering background texture: {}", SDL_GetError());
            }
        }
    }
}

void minesweeper::App::render_mines() const {
    for (int i{}; i < M_ROW_NUMBER_CELLS; ++i) {
        for (int j{}; j < M_COLUMN_NUMBER_CELLS; ++j) {
            if (m_map.at(i).at(j) != -1) { continue; }

            SDL_FRect dstrect{
                .x{ j * CELL_SIZE },
                .y{ i * CELL_SIZE },
                .w{ CELL_SIZE },
                .h{ CELL_SIZE }
            };

            if (!SDL_RenderTexture(m_renderer, m_mine_texture, nullptr, &dstrect)) {
                throw std::format("Error rendering mine texture: {}", SDL_GetError());
            }
        }
    }
}

void minesweeper::App::render_numbers() const {
    for (int i{}; i < M_ROW_NUMBER_CELLS; ++i) {
        for (int j{}; j < M_COLUMN_NUMBER_CELLS; ++j) {
            SDL_FRect dstrect{
                .x{ j * CELL_SIZE + 6.0f },
                .y{ i * CELL_SIZE + 1.5f },
                .w{ CELL_SIZE - 8.0f },
                .h{ CELL_SIZE }
            };

            const int CELL{ m_map.at(i).at(j) };
            if (CELL < 1) { continue; }
            if (!SDL_RenderTexture(m_renderer, m_numbers_textures.at(CELL - 1), nullptr, &dstrect)) {
                throw std::runtime_error(std::format("Error rendering number texture: {}", SDL_GetError()));
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
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                    for (int i{}; i < m_buttons.size(); ++i) {
                        if (!m_buttons.at(i).update()) {
                            m_buttons.erase(m_buttons.begin() + i, m_buttons.begin() + i + 1);
                        }
                    }
                    break;
                default:
                    break;
            }
        }
        
        if (!SDL_RenderClear(m_renderer)) {
            throw std::runtime_error(std::format("Error in render clear: {}", SDL_GetError()));
        }

        render_background();
        render_mines();
        render_numbers();
        for (const Button button : m_buttons) { button.render_button(m_renderer); }

        if (!SDL_RenderPresent(m_renderer)) {
            throw std::runtime_error(std::format("Error in render present: {}", SDL_GetError()));
        }
    }
}