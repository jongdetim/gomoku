#ifndef GUI_HPP
# define GUI_HPP

# include <iostream>
# include <SDL.h>
# include <SDL_image.h>
# include <SDL_ttf.h>
# include "Board.hpp"

# define OFFSET 31
# define SIZE 50
# define INTERFACE_SIZE 420
# define SCREEN_HEIGHT 960
# define SCREEN_WIDTH (SCREEN_HEIGHT + INTERFACE_SIZE)
# define BOARD_PATH "../textures/board.png"
# define P1_PATH "../textures/circle_white.png"
# define P2_PATH "../textures/circle_black.png"
# define BG_COLOUR {220, 179, 92, 255}
# define BUTTON_BIG(x,y) {x, y, this->interface_size - ((int)this->offset << 1), this->size << 1}
# define BUTTON_LEFT(x,y) {x, y, (int)(((this->interface_size - ((int)this->offset << 1)) >> 1) + .5) - (this->size >> 1), this->size << 1}
# define BUTTON_RIGHT(x,y) {x + (int)(((this->interface_size - ((int)this->offset << 1)) >> 1) + .5) + (this->size >> 1), y, (int)(((this->interface_size - ((int)this->offset << 1)) >> 1) + .5) - (this->size >> 1), this->size << 1}

enum gui_size
{
	big,
	medium,
	small
};

typedef struct s_button
{
	SDL_Rect	rec;
	SDL_Colour	col;
}				t_button;

class GUI
{
public:
	GUI(void);
	GUI(gui_size size);
	~GUI();
	bool					initiate_GUI(std::string title);
	void					game(Board &board);

private:
	SDL_Window				*window;
	SDL_Renderer			*renderer;
	SDL_Texture				*board_texture;
	SDL_Texture				*p1_texture;
	SDL_Texture				*p2_texture;
	SDL_Event				event;
	bool					update;
	int						screen_height;
	int						screen_width;
	int						size;
	int						interface_size;
	double					offset;
	std::vector<t_button>	buttons;

	void					reset(Board &board);
	bool					mouse_on_board(int row, int col);
	void					clear_render(void);
	void					set_texture(SDL_Texture *texture, SDL_Rect rect);
	void					draw_stones(Board &board);
	void					update_renderer(Board &board);
	void					draw_interface(Board &board);
	bool					handle_events(Board &board, int &index);
	int						calc_board_placement(int row, int col);
	SDL_Texture				*load_texture(std::string img_path);
	SDL_Colour				subtract_colour(SDL_Colour col, int amount) const;
	// void					place_button(SDL_Colour col, SDL_Rect rect);
	// void					initiate_buttons(void);
	bool					in_square(int x, int y, SDL_Rect rec);
};

#endif