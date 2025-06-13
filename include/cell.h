#ifndef __CELL__
#define __CELL__

#include <SDL.h>

class Cell
{
	bool alive;
	SDL_Rect cell;
	SDL_Color color;
	static inline constexpr SDL_Color BLACK{0, 0, 0, 255};
    static inline constexpr SDL_Color WHITE{255, 255, 255, 255};
public:
	Cell(int x, int y, int w, int h);
	void setAlive(bool alive);
	inline bool isAlive() const { return alive; }
	inline const SDL_Rect& getRect() const { return cell; }
	inline const SDL_Color& getColor() const { return color; }
	void draw(SDL_Renderer *renderer) const;

	bool operator==(const Cell& other) const { return cell.x == other.cell.x && cell.y == other.cell.y; }
};

#endif