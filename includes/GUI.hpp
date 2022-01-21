#ifndef GUI_HPP
# define GUI_HPP

# include <iostream>
# include <SDL.h>
# include <SDL_image.h>
# include <SDL_ttf.h>
# include "Board.hpp"
# include "Button.hpp"
# include "Stats.hpp"

# define OFFSET 31
# define SIZE 50
# define INTERFACE_SIZE 420
# define SCREEN_HEIGHT 960
# define SCREEN_WIDTH (SCREEN_HEIGHT + INTERFACE_SIZE)
# define BOARD_PATH "../textures/board.png"
# define P1_PATH "../textures/circle_white.png"
# define P2_PATH "../textures/circle_black.png"
# define AMATIC_FONT "../fonts/Amatic-Bold.ttf"
# define SANS_FONT "../fonts/OpenSans-SemiBold.ttf"
# define ROBOTO_FONT "../fonts/Roboto-Bold.ttf"
# define MONTSER_FONT "../fonts/Montserrat-Medium.ttf"
# define SCPRO_FONT "../fonts/SourceCodePro-Regular.ttf"
# define BG_COLOUR SDL_Color{220, 179, 92, 255}
# define BTN_COLOUR SDL_Color{180, 60, 60, 255}
# define BUTTON_SIZE 25
# define STATS_SIZE 20

enum gui_size
{
	big,
	medium,
	small
};

enum action
{
	def,
	restart,
	pause
};

class GUI: public IGameEngine
{
public:
	GUI(void);
	GUI(gui_size size);
	~GUI();

	bool					init(std::string title);
	void					play(Board *board);

private:
	SDL_Window				*window;
	SDL_Renderer			*renderer;
	SDL_Texture				*board_texture;
	SDL_Texture				*p1_texture;
	SDL_Texture				*p2_texture;
	SDL_Event				event;
	TTF_Font				*btn_font;
	TTF_Font				*stats_font;

	std::vector<Button>		buttons;
	Stats					statsP1;
	Stats					statsP2;
	Text					status;

	bool					update;
	int						screen_height;
	int						screen_width;
	int						size;
	int						interface_size;
	double					offset;
	int						btn_size;
	int						stats_size;
	short					action;

	void					gameloop(Board &board);
	void					reset(Board &board);
	bool					mouse_on_board(int row, int col) const;
	void					clear_render(void);
	void					set_texture(SDL_Texture *texture, SDL_Rect rect);
	void					draw_stones(Board &board);
	void					update_renderer(Board &board);
	bool					handle_events(Board &board, int &index);
	int						calc_board_placement(int row, int col) const;
	SDL_Texture				*load_texture(std::string img_path);
	void					load_textures(void);
	void					set_buttons(void);
	void					render_buttons(void);
	void					check_action(Board &board);
	void					init_stats(Board &board);
	void					show_stats(void);
	void					update_status(Board &board);
};

#endif