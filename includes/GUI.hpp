#ifndef GUI_HPP
# define GUI_HPP

# include <iostream>
# include <SDL2/SDL.h>
# include "Board.hpp"

# define SCREEN_WIDTH 1280
# define SCREEN_HEIGHT 960

enum game_loop
{
	quit,
	loop
};

class GUI
{
public:
	GUI(void);
	~GUI();
	bool			initiate_GUI(std::string title, int width, int height);
	bool			initiate_GUI(std::string title);
	void			game(const Board &board);

private:
	SDL_Window		*window;
	SDL_Renderer	*renderer;

	void			draw_state(BITBOARD state);
	void			update_window(void);
	game_loop		handle_events(SDL_Event &e);



};

#endif