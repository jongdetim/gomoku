#ifndef STATS_HPP
# define STATS_HPP

# include <iostream>
# include <SDL.h>
# include <SDL_ttf.h>
# include "Player.hpp"
# include "Button.hpp"
# include "Text.hpp"
# include <sstream>

class Stats
{
public:
	Stats(void);
	Stats(SDL_Renderer *renderer, Player *player, t_point pos, TTF_Font *font);

	void				update(void);
	void				render(void);
	void				init(void);
private:

	Player				*player;
	SDL_Renderer		*renderer;
	std::vector<Text>	texts;
	t_point				pos;
	TTF_Font			*font;
	int 				textH;
};

#endif