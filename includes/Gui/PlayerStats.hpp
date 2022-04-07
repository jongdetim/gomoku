/* MIT License

Copyright (c) 2022 Flint Louis

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
#ifndef PLAYERSTATS_HPP
# define PLAYERSTATS_HPP

# include <SDL.h>
# include <SDL_ttf.h>
# include "GuiPlayer.hpp"
# include "Text.hpp"

enum e_playerstats
{
	name_text,
	hint_text,
	player_text,
	captures_text,
	/* ----- */
	size_playertexts
};

class PlayerStats
{
public:
	PlayerStats(void);
	PlayerStats(SDL_Renderer *renderer, t_point pos, TTF_Font *font, TTF_Font *name_font);

	void				update_font(int index, TTF_Font *font);
	void				update(GuiPlayer &player);
	void				render(void);
	void				init(void);
	t_point				get_position(int index) const;
	t_point				get_size(e_playerstats index) const;
	bool				on_text(int x, int y, e_playerstats index);
	bool				is_active(e_playerstats index) const;
	void				free(void);
	
private:

	Text				texts[size_playertexts];
	SDL_Renderer		*renderer;
	t_point				pos;
	TTF_Font			*font;
	TTF_Font			*name_font;
	int 				textH;
};

#endif