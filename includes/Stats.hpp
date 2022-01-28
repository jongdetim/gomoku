#ifndef STATS_HPP
# define STATS_HPP

# include <iostream>
# include <SDL.h>
# include <SDL_ttf.h>
# include "Player.hpp"
# include "Button.hpp"
# include "Text.hpp"
# include <sstream>

enum e_stats
{
	name_text,
	empty_text,
	captures_text,
	stones_text,
	ai_text,
	/* ----- */
	size_texts
};

class Stats
{
public:
	Stats(void);
	Stats(SDL_Renderer *renderer, t_point pos, TTF_Font *font, TTF_Font *name_font);

	void				update(Player &player, int current_id);
	void				render(void);
	void				init(void);
private:

	SDL_Renderer		*renderer;
	std::vector<Text>	texts;
	t_point				pos;
	TTF_Font			*font;
	TTF_Font			*name_font;
	int 				textH;
};

#endif