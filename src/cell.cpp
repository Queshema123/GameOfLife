#include "cell.h"

Cell::Cell(int x, int y, int w, int h) : color{BLACK}, cell{x, y, w, h}, alive{false} { }

void Cell::setAlive(bool alive)
{
    this->alive = alive;
    this->color  = alive ? WHITE : BLACK;
}

void Cell::draw(SDL_Renderer *renderer) const
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &cell);
}