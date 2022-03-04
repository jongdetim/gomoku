#ifndef BUTTON_HPP
# define BUTTON_HPP

# include <iostream>
# include <SDL.h>
# include <SDL_image.h>
# include <SDL_ttf.h>

# define SUBTRACT	30

typedef struct	s_point
{
	int 		x;
	int 		y;
}				t_point;

class Button
{
public:
	Button(SDL_Renderer *renderer, int x, int y, std::string text, TTF_Font *font, SDL_Color colour, short action);
	~Button();

	void					render(void);
	bool					on_button(int x, int y);
	bool					is_active(void);
	short					get_action(void);
	void					init(void);
	t_point					get_button_size(void);

private:
	bool					active;
	SDL_Texture				*texture;
	t_point					pos;
	std::string				text;
	SDL_Color				colour;
	SDL_Rect				rect;
	SDL_Renderer			*renderer;
	short					action;
	TTF_Font				*font;

	SDL_Colour				subtract_colour(SDL_Colour col, int amount) const;
};

#endif
