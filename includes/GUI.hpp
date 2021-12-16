#ifndef GUI_HPP
# define GUI_HPP

# include <iostream>
# include <SDL2/SDL.h>
# include "Board.hpp"


// # define OFFSET 0.03229
# define OFFSET 31
# define SIZE 50
# define INTERFACE_SIZE 320
# define SCREEN_HEIGHT 960
# define SCREEN_WIDTH (SCREEN_HEIGHT + INTERFACE_SIZE)
# define BOARD_PATH "../textures/board.bmp"
# define P1_PATH "../textures/circle_white.bmp"
# define P2_PATH "../textures/circle_black.bmp"

class GUI
{
public:
	GUI(void);
	~GUI();
	bool			initiate_GUI(std::string title);
	void			game(Board &board);

private:
	SDL_Window		*window;
	SDL_Renderer	*renderer;
	SDL_Texture		*board_texture;
	SDL_Texture		*p1_texture;
	SDL_Texture		*p2_texture;
	SDL_Event		event;
	bool			update;

	void			clear_render(void);
	void			set_texture(SDL_Texture *texture, SDL_Rect rect);
	void			draw_stones(Board &board);
	void			update_renderer(Board &board);
	bool			handle_events(Board &board);
	void			get_placement(int *row, int *col);
	SDL_Texture		*load_texture(std::string img_path);

};

#endif