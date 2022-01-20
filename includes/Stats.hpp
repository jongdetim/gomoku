#ifndef STATS_HPP
# define STATS_HPP

# include <iostream>
# include <SDL.h>
# include <SDL_ttf.h>
# include <Eigen/Dense>
# include "Player.hpp"

class Stats
{
public:
	Stats(void);
	Stats(SDL_Renderer *renderer, Player *player, int x, int y, TTF_Font *font);

	void				update(void);
	void				render(void);
	void				init(void);

private:
	class Text
	{
	public:
		Text(int x, int y, TTF_Font *font);
		~Text();

		void			update(std::string text, SDL_Renderer*renderer);
		void			render(SDL_Renderer*renderer);
	private:
		SDL_Texture		*texture;
		Eigen::Array2i	pos;
		TTF_Font		*font;
	};

private:

	Player				*player;
	SDL_Renderer		*renderer;
	std::vector<Text>	texts;
	Eigen::Array2i		pos;
	TTF_Font			*font;

};

#endif