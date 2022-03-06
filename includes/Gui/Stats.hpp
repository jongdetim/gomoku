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
	t_point				get_position(e_stats index) const;
	t_point				get_size(e_stats index) const;
	bool				on_text(int x, int y, e_stats index) const;
	
private:

	std::vector<Text>	texts;
	SDL_Renderer		*renderer;
	t_point				pos;
	TTF_Font			*font;
	TTF_Font			*name_font;
	int 				textH;
};

#endif