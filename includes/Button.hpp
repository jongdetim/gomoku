#ifndef BUTTON_HPP
# define BUTTON_HPP

# include <iostream>
# include <SDL.h>
# include <SDL_image.h>
# include <SDL_ttf.h>
# include <Eigen/Dense>

# define RESET		1
# define NEWGAME	2

class Button
{
public:
	Button(SDL_Renderer *renderer, int x, int y, std::string text, TTF_Font *font, SDL_Color colour, short action);
	~Button();

	void					render(void);
	bool					on_button(int x, int y);
	bool					is_active(void);
	short					get_action(void);
	void					init_button(void);

	SDL_Texture				*texture;
private:
	Eigen::Array2i			pos;
	std::string				text;
	SDL_Color				colour;
	bool					active;
	SDL_Rect				rect;
	SDL_Renderer			*renderer;
	short					action;
	TTF_Font				*font;

	SDL_Colour				subtract_colour(SDL_Colour col, int amount) const;
};

#endif
