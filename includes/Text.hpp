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
	Text(SDL_Renderer *renderer, t_point pos, TTF_Font *font);
	~Text();

	void			update(std::string text);
	void			render(void);
private:
	SDL_Renderer	*renderer;
	SDL_Texture		*texture;
	t_point			pos;
	TTF_Font		*font;
};

#endif