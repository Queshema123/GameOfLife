#ifndef __GAME__
#define __GAME__

#include <array>
#include <unordered_set>
#include <SDL.h>

class Game
{
    std::array<int, 8> neighbor_offsets;
    std::unordered_set<std::size_t> alive_ceils;              
    
    bool isPaused;
    int game_speed;
    int ROWS, COLS;
    int ceil_size;
    int width, height;

    SDL_Renderer* renderer;
    SDL_Window* window;

    void initOffsets();
protected:
    bool initGUI(int w, int h);
    void processKeyboardEvents(const SDL_Event& event);
public:
    Game(int w, int h, int cell_size, int speed);
    ~Game();
    
    void drawField(SDL_Renderer *renderer, int width, int height, int ceil_size);
    void drawAliveCeils(SDL_Renderer *renderer, int width, int height, int ceil_size);
    void updateCellsStates(); // Update active_cells and their neighbor states
    void changeCellState(int x, int y, bool state); // Make dead or alive cell by coords
    void draw(); // Draw field and cells
    bool loop(); // Game loop
    void start();

    inline void setPause(bool pause) { isPaused = pause; }
};

#endif