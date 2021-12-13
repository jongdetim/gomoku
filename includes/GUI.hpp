#ifndef GUI_HPP
# define GUI_HPP

# include <iostream>
# include <SDL2/SDL.h>
# include "Board.hpp"


// # define OFFSET 0.03229
# define OFFSET 31
# define GAP 50
# define INTERFACE_SIZE 320
# define SCREEN_HEIGHT 960
# define SCREEN_WIDTH (SCREEN_HEIGHT + INTERFACE_SIZE)
# define IMAGE_PATH "../board.bmp"

class GUI
{
public:
	GUI(void);
	~GUI();
	bool			initiate_GUI(std::string title);
	void			game(const Board &board);

private:
	SDL_Window		*window;
	SDL_Renderer	*renderer;
	SDL_Surface		*image;
	SDL_Texture		*texture;
	SDL_Event		event;
	bool			update;

	void			clear_render(void);
	void			set_background(void);
	void			draw_state(BITBOARD state);
	void			update_window(void);
	bool			handle_events(void);
	void			get_placement(int *row, int *col);
	void			draw_circle(int32_t centreX, int32_t centreY, int32_t radius);

};

#endif