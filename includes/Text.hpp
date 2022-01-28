#ifndef TEXT_HPP
# define TEXT_HPP

# include <iostream>
# include <SDL.h>
# include <SDL_ttf.h>
# include "Button.hpp"

class Text
{
public:
	Text(void);
	Text(SDL_Renderer *renderer, t_point pos);
	~Text();

	void			update(std::string text, TTF_Font *font);
	void			render(void);
private:
	SDL_Renderer	*renderer;
	SDL_Texture		*texture;
	t_point			pos;
};

#endif