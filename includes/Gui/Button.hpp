/* MIT License

Copyright (c) 2022 Flint Louis, Tim de Jong

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 */
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

	void					render(void);
	bool					on_button(int x, int y);
	bool					is_active(void);
	short					get_action(void);
	void					init(void);
	t_point					get_button_size(void);
	void					free(void);

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
	t_point					size;

	SDL_Colour				subtract_colour(SDL_Colour col, int amount) const;
};

#endif
