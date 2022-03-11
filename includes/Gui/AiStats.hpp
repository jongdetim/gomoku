#ifndef AISTATS_HPP
# define AISTATS_HPP

# include <SDL.h>
# include <SDL_ttf.h>
# include "Text.hpp"
# include "algorithm.hpp"

class AiStats
{
public:
	AiStats(void);
	AiStats(SDL_Renderer *renderer, SDL_Rect rect, TTF_Font *font);

	t_aistats			stats;

	void				render(t_aistats stats);
	void				init(int offset);
	
private:
	void				update(t_aistats stats);

	SDL_Rect			rect;
	Text				duration;
	Text				depth;
	Text				nodes;
	Text				score;
	Text				nodes_ps;
	SDL_Renderer		*renderer;
	TTF_Font			*font;
	int 				textH;
	t_point				pos;
};

#endif