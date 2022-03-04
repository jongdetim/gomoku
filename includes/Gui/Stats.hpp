#ifndef STATS_HPP
# define STATS_HPP

# include <iostream>
# include <SDL.h>
# include <SDL_ttf.h>
# include "GuiPlayer.hpp"
# include "Button.hpp"
# include "Text.hpp"
# include <sstream>

enum e_stats
{
	name_text,
	empty_text,
	player_text,
	captures_text,
	/* ----- */
	size_texts
};

class Stats
{
public:
	Stats(void);
	Stats(SDL_Renderer *renderer, t_point pos, TTF_Font *font, TTF_Font *name_font);

	void				update(GuiPlayer &player);
	void				render(void);
	void				init(void);
	t_point				get_position(int index) const;
	t_point				get_size(int index) const;
private:

	SDL_Renderer		*renderer;
	std::vector<Text>	texts;
	t_point				pos;
	TTF_Font			*font;
	TTF_Font			*name_font;
	int 				textH;
};

#endif