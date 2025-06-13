#ifndef __GAME__
#define __GAME__

#include <vector>
#include <functional>
#include <bitset>

#include "game_field.h"
#include "cell.h"


class Game
{
    std::vector<std::pair<int, int>> alive_cells_pos;
    std::vector<std::vector<Cell>> cells;
    GameField field;

    bool isPaused;
    int game_speed;
    int max_row_cell_pos, max_col_cell_pos;

    SDL_Renderer* renderer;
    SDL_Window* window;
protected:
    bool initGUI(int w, int h);
    void createCells(int w, int h, int size);
    void processKeyboardEvents(const SDL_Event& event);
    void setCellState(const std::pair<int, int> &pos, bool alive);
    void addActiveCell(const std::pair<int, int> &pos);
    void delActiveCell(const std::pair<int, int> &pos);
public:
    Game(int w, int h, int cell_size, int speed);
    ~Game();

    bool isAliveCell(const std::pair<size_t, size_t>& pos, const std::vector<std::pair<int, int>>& neighbors); // Check cells game logic
    std::vector<std::pair<int, int>> getNeighborsPos(const std::pair<int, int> &pos);
    
    void updateCellsStates(); // Update active_cells and their neighbor states
    void changeCellState(int x, int y, bool state); // Make dead or alive cell by coords
    void draw(); // Draw field and cells
    bool loop(); // Game loop
    void start();

    inline void setPause(bool pause) { isPaused = pause; }
};

#endif