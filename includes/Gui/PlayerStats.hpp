#ifndef PLAYERSTATS_HPP
# define PLAYERSTATS_HPP

# include <SDL.h>
# include <SDL_ttf.h>
# include "GuiPlayer.hpp"
# include "Text.hpp"

enum e_playerstats
{
	name_text,
	empty_text,
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

	void				update(GuiPlayer &player);
	void				render(void);
	void				init(void);
	t_point				get_position(int index) const;
	t_point				get_size(e_playerstats index) const;
	bool				on_text(int x, int y, e_playerstats index) const;
	
private:

	Text				texts[size_playertexts];
	SDL_Renderer		*renderer;
	t_point				pos;
	TTF_Font			*font;
	TTF_Font			*name_font;
	int 				textH;
};

#endif