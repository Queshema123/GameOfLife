#include "game.h"
#include <iostream>
#include <algorithm>
#include <SDL.h>

Game::Game(int w, int h, int cell_size, int speed) : field{w, h, cell_size, {255, 0, 0}}, isPaused{false}, game_speed{speed}, 
    window{nullptr}, renderer{nullptr}, max_row_cell_pos{0}, max_col_cell_pos{0}
{ 
    if( !initGUI(w, h) )
        throw std::runtime_error("Can't initialize GUI");

    createCells(w, h, cell_size);
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

void Game::createCells(int w, int h, int size)
{
    int rows{ w / size };
    int cols{ h / size };
    max_row_cell_pos = rows;
    max_col_cell_pos = cols;

    cells.reserve(rows);
    for(int i{0}; i < rows; ++i)
    {
        std::vector<Cell> tmp;
        tmp.reserve(cols);
        for(int j{0}; j < cols; ++j)
            tmp.push_back(Cell(i*size, j*size, size, size));
        
        cells.push_back(std::move(tmp));
    }
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

    SDL_RenderPresent(renderer);
    return true;
}

void Game::draw()
{   
    for(int i{0}; i < cells.size(); ++i)
        for(int j{0}; j < cells[i].size(); ++j)
            cells[i][j].draw(renderer);

    field.draw(renderer);
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

        SDL_Delay(100 / game_speed);
    }
}

std::vector<std::pair<int, int>> Game::getNeighborsPos(const std::pair<int, int> &pos)
{
    static auto inBound = [](int r, int c, int maxr, int maxc) -> bool{
        return (unsigned int)r < maxr && (unsigned int)c < maxc; 
    };

    constexpr int MAX_NEIGHBOR_POS{2}, MIN_NEIGHBOR_POS{-1}; // max and mix indexes of square neighbors, they position located in cell.pos+-1 in row and col
    std::vector<std::pair<int, int>> neighbors;
    for(int i{MIN_NEIGHBOR_POS}; i < MAX_NEIGHBOR_POS; ++i)
    {
        for(int j{MIN_NEIGHBOR_POS}; j < MAX_NEIGHBOR_POS; ++j)
        {
            if( i == 0 && j == 0 ) // - pos of checked cell
                continue;
            int r{pos.first+i}, c{pos.second+j};
            if( inBound(r, c, max_row_cell_pos, max_col_cell_pos) )
                neighbors.push_back( {pos.first + i, pos.second + j} );
        }
    }

    return neighbors;
}

bool Game::isAliveCell(const std::pair<size_t, size_t>& pos, const std::vector<std::pair<int, int>>& neighbors)
{
    int cntAliveNeighbors{0};
    for(const auto& pos: neighbors)
    {
        if(cells[pos.first][pos.second].isAlive())
            ++cntAliveNeighbors;
    }

    if(cntAliveNeighbors > 3 || cntAliveNeighbors < 2)
        return false;

    return true;
}

void Game::updateCellsStates()
{
    std::vector<std::pair<int, int>> neighborOfAliveCells;
    for(auto it=alive_cells_pos.begin(); it != alive_cells_pos.end(); )
    {
        auto pos{ *it };
        auto neighbors = getNeighborsPos(pos);
        if( !isAliveCell(pos, neighbors) )
        {
            setCellState(pos, false);
            it = alive_cells_pos.erase(it);
        }
        else
            neighborOfAliveCells.insert(neighborOfAliveCells.end(), neighbors.begin(), neighbors.end());
    }    

    for(const auto& pos: neighborOfAliveCells)
        if( isAliveCell(pos, getNeighborsPos(pos)) )
            addActiveCell(pos);
}

void Game::changeCellState(int x, int y, bool alive)
{
    int r{ x / field.getCellSize() }, c{ y / field.getCellSize() };
    if(alive)
        addActiveCell({r, c});
    else
        delActiveCell({r, c});
}

void Game::setCellState(const std::pair<int, int> &pos, bool alive)
{
    cells[pos.first][pos.second].setAlive(alive);
    cells[pos.first][pos.second].draw(renderer);
}

void Game::addActiveCell(const std::pair<int, int> &pos)
{
    setCellState(pos, true);
    alive_cells_pos.push_back(pos);
}

void Game::delActiveCell(const std::pair<int, int> &pos)
{
    setCellState(pos, false);
    alive_cells_pos.erase( std::remove(alive_cells_pos.begin(), alive_cells_pos.end(), pos), alive_cells_pos.end() );
}

Game::~Game()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}