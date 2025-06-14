#include "game.h"
#include <iostream>
#include <unordered_map>
#include <SDL.h>

Game::Game(int w, int h, int cell_size, int speed) : ceil_size{cell_size}, width{w}, height{h},
    isPaused{false}, game_speed{speed}, window{nullptr}, renderer{nullptr}, COLS{w/cell_size},
    ROWS{h/cell_size}
{ 
    if( !initGUI(w, h) )
        throw std::runtime_error("Can't initialize GUI");

    initOffsets();
}

void Game::initOffsets()
{
    neighbor_offsets = {
            -COLS - 1,
            -COLS,
            -COLS + 1,
            -1,
            1,
            COLS - 1,
            COLS,
            COLS + 1
    };
}

bool Game::initGUI(int w, int h)
{
    if( SDL_Init( SDL_INIT_EVERYTHING ) < 0)
    {
        std::cerr << "Error initializing SDL: " << SDL_GetError() << std::endl;
        return false;
    }
    window = SDL_CreateWindow( "window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN );
    if( !window )
    {
        std::cerr << "Failed to create a window! Error: " << SDL_GetError() << std::endl;
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if( !renderer )
    {
        std::cerr << "Failed to create a renderer! Error: " << SDL_GetError() << std::endl;
        return false;
    }
    return true;
}

void Game::processKeyboardEvents(const SDL_Event& event)
{
    switch (event.key.keysym.sym)
    {
    case SDLK_ESCAPE:
        setPause(!isPaused);
        break;
    } 
}

bool Game::loop()
{
    SDL_Event event;
    
    while( SDL_PollEvent(&event) != 0)
    {
        switch (event.type)
        {
        case SDL_QUIT:
            return false;
        case SDL_MOUSEBUTTONDOWN:
            changeCellState(event.button.x, event.button.y, event.button.button == SDL_BUTTON_LEFT);
            break;
        case SDL_KEYDOWN:
            processKeyboardEvents(event);
            break;
        }
    }

    return true;
}

void Game::drawField(SDL_Renderer *renderer, int width, int height, int ceil_size)
{
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red color for lines

    int rows{height / ceil_size};
    for(int i = 0; i < rows; ++i)
        SDL_RenderDrawLine(renderer,  0, i*ceil_size, width, i*ceil_size); // Draw horizontal lines with step cell_size

    int cols{width / ceil_size};
    for(int i = 0; i < cols; ++i)
        SDL_RenderDrawLine(renderer, i*ceil_size, 0, i*ceil_size, height); // Draw vertical lines with step cell_size
}

void Game::drawAliveCeils(SDL_Renderer *renderer, int width, int height, int ceil_size)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for (auto ceil : alive_ceils)
    {
        int r = ceil / COLS;
        int c = ceil % COLS; 

        SDL_Rect rect;
        rect.x = c * ceil_size;
        rect.y = r * ceil_size;
        rect.w = ceil_size;
        rect.h = ceil_size;

        SDL_RenderFillRect(renderer, &rect);
    }
}

void Game::draw()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    drawField(renderer, width, height, ceil_size);
    drawAliveCeils(renderer, width, height, ceil_size);

    SDL_RenderPresent(renderer);
}

void Game::start()
{
    while( loop() )
    {
        if(!isPaused) // if users add new active cell or old cell change state to alive
            updateCellsStates();

        // Drawing section
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        draw();
        SDL_RenderPresent(renderer);
        // end

        SDL_Delay(1000 / game_speed);
    }
}

void Game::updateCellsStates()
{
    std::unordered_map<std::size_t, int> neighbor_counts;

    for (std::size_t cell : alive_ceils)
    {
        for (int offset : neighbor_offsets)
        {
            std::size_t neighbor = cell + offset;
            ++neighbor_counts[neighbor];
        }
    }

    std::unordered_set<std::size_t> next_gen;

    for (const auto& [cell, count] : neighbor_counts)
    {
        if (count == 3 || (count == 2 && alive_ceils.count(cell)))
            next_gen.insert(cell);
    }

    alive_ceils = std::move(next_gen);
}

void Game::changeCellState(int x, int y, bool alive)
{
    int r{ y / ceil_size }, c{ x / ceil_size };
    if(alive)
        alive_ceils.insert(r * COLS + c );
    else
        alive_ceils.erase(r * COLS + c );
}

Game::~Game()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}