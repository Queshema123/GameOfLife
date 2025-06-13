#ifndef __GAME_FIELD__
#define __GAME_FIELD__

#include "cell.h"
#include <SDL.h>

class GameField
{
    int width, height, cell_size;
    SDL_Color line_color;
public:
    GameField(int width, int height, int cell_size, SDL_Color color);
    void draw(SDL_Renderer* renderer) const;
    inline void setSize(int w, int h) { width = w; height = h; }
    inline int getWidth() const { return width; }
    inline int getHeight() const { return height; }
    inline int getCellSize() const { return cell_size; }
};

#endif