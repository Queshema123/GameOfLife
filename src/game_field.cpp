#include "game_field.h"

GameField::GameField(int width, int height, int cell_size, SDL_Color color): 
    width{width}, height{height}, cell_size{cell_size}, line_color{color} { }

void GameField::draw(SDL_Renderer* renderer) const
{
    // Set red color for grid lines
    SDL_SetRenderDrawColor(renderer, line_color.r, line_color.g, line_color.b, line_color.a);

    int rows{height / cell_size};
    for(int i = 0; i < rows; ++i)
        SDL_RenderDrawLine(renderer,  0, i*cell_size, width-1, i*cell_size); // Draw horizontal lines with step cell_size

    int cols{width / cell_size};
    for(int i = 0; i < cols; ++i)
        SDL_RenderDrawLine(renderer, i*cell_size, 0, i*cell_size, height-1); // Draw vertical lines with step cell_size
}
