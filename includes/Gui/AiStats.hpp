#ifndef AISTATS_HPP
# define AISTATS_HPP

# include <SDL.h>
# include <SDL_ttf.h>
# include "Text.hpp"
# include <vector>
# include "algorithm.hpp"

enum e_aistats
{
	time_text,
	depth_text,
	/* ----- */
	size_aitexts
};

class AiStats
{
public:
	AiStats(void);
	AiStats(SDL_Renderer *renderer, SDL_Rect rect, TTF_Font *font);

	t_aistats			stats;

// 	void				update(GuiPlayer &player);
	void				render(void);
	void				init(void);
// 	t_point				get_position(e_stats index) const;
// 	t_point				get_size(e_stats index) const;
// 	bool				on_text(int x, int y, e_stats index) const;
	
private:

	SDL_Rect			rect;
	std::vector<Text>	texts;
	SDL_Renderer		*renderer;
	TTF_Font			*font;
	int 				textH;
	t_point				pos;
};

#endif