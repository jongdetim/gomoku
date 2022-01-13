#ifndef BUTTON_HPP
# define BUTTON_HPP

# include <iostream>
# include <SDL.h>
# include <SDL_image.h>
# include <SDL_ttf.h>
# include <Eigen/Dense>

class Button
{
public:
	Button(void);
	Button(SDL_Renderer *renderer, Eigen::Array2i pos, std::string text, int size, std::string font, SDL_Color colour);
	Button(SDL_Renderer *renderer, int x, int y, std::string text, int size, std::string font, SDL_Color colour);

	void	render(void);
	bool	is_active(int x, int y);

	TTF_Font				*font;
	SDL_Texture				*texture;
private:
	Eigen::Array2i			pos;
	std::string				text;
	SDL_Color				colour;
	bool					active;
	SDL_Rect				rect;
	SDL_Renderer			*renderer;

	SDL_Colour				subtract_colour(SDL_Colour col, int amount) const;
	void					init_button(void);
};

#endif
