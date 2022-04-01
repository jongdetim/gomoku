#ifndef GUI_HPP
# define GUI_HPP

# include <SDL.h>
# include <SDL_ttf.h>
# include "IGameEngine.hpp"
# include "Board.hpp"
# include "Button.hpp"
# include "PlayerStats.hpp"
# include "AiStats.hpp"
# include "GuiBoard.hpp"
#include <future>

# define OFFSET 31
# define SIZE 50
# define INTERFACE_SIZE 420
# define SCREEN_HEIGHT 960
# define SCREEN_WIDTH (SCREEN_HEIGHT + INTERFACE_SIZE)

# define BG_COLOUR SDL_Color{220, 179, 92, 255}
# define BTN_COLOUR SDL_Color{180, 60, 60, 255}
# define BUTTON_SIZE 25
# define STATS_SIZE 23
# define STATUS_SIZE 30
# define TITLE_SIZE 55

# define NAMES "resources/names.txt"

# define BOARD_PATH "resources/textures/board.png"
# define P1_PATH "resources/textures/circle_white.png"
# define P2_PATH "resources/textures/circle_black.png"
# define P1_SELECT "resources/textures/select_black.png"
# define P2_SELECT "resources/textures/select_white.png"
# define WIN_SELECT "resources/textures/red_circle.png"
# define RED_CROSS "resources/textures/red_cross.png"

# define SANS_FONT "resources/fonts/OpenSans/Regular.ttf"
# define SANS_FONT_SEMI "resources/fonts/OpenSans/SemiBold.ttf"
# define SANS_FONT_BOLD "resources/fonts/OpenSans/Bold.ttf"
# define ALLER_FONT "resources/fonts/Aller/Regular.ttf"
# define POPPINS_FONT_BOLD "resources/fonts/Poppins/Bold.otf"

# define BTN_FONT ALLER_FONT
# define STATS_FONT SANS_FONT
# define STATS_FONT_SELECT SANS_FONT_SEMI
# define STATS_NAME_FONT SANS_FONT_BOLD
# define STATUS_FONT POPPINS_FONT_BOLD
# define TITLE_FONT "resources/fonts/Title.ttf"

# define BOARD_DATA_FILE_EXT ".board.data"

# define FPS 20
# define HINT_TIMEOUT 20000

class	NegamaxAi;

enum e_fonts
{
	btn_font,
	aistats_font,
	stats_font,
	stats_select_font,
	stats_name_font,
	status_font,
	title_font,
	/* ----- */
	size_font
};

enum e_textures
{
	board_tex,
	p1_tex,
	p2_tex,
	p1_select_tex,
	p2_select_tex,
	winning_tex,
	cross_tex,
	/* ----- */
	size_tex
};

enum e_gui_size
{
	big,
	medium,
	small
};

typedef struct	s_mouse
{
	t_point		pos;
	bool 		clicked;
}				t_mouse;

typedef struct	s_config
{
	int			screen_height;
	int			screen_width;
	int			size;
	int			interface_size;
	int			btn_size;
	int			stats_size;
	int			status_size;
	int			title_size;
	double		offset;
}				t_config;

typedef struct	s_replay
{
	int			starting_id;
	int			current_id;
	std::string	dir;
}				t_replay;

class GUI: public IGameEngine
{
public:
	GUI(void);
	GUI(NegamaxAi *ai);
	GUI(NegamaxAi *ai, e_gui_size size);
	~GUI();

	void					play(Board board);
	void					replay(std::string board_data_path);

private:
	GuiBoard				guiboard;
	GuiBoard				prev;

	SDL_Window				*window;
	SDL_Renderer			*renderer;

	t_config				config;
	std::future<t_aistats>	task;
	std::atomic<int>		move_highlight = -1;
	std::atomic<bool>		stop_search = false;
	
	TTF_Font				*fonts[size_font];
	SDL_Texture				*textures[size_tex];
	PlayerStats				player_stats[2];
	AiStats					ai_stats;
	std::vector<Button>		buttons;
	Text					title;
	Text					status;

	bool					button_pressed;
	bool					update;
	int						action;
	Uint32					ticks;
	t_mouse					mouse;

	bool					replay_mode;
	t_replay				replay_settings;

	bool					init(std::string title);
	void					gameloop(void);
	void					place_stone(void);
	void					init_game(void);
	void					reset(void);

	/* Event methods */
	void					handle_events(void);
	void					key_press(int key);

	/* Render methods */
	void					update_renderer(void);
	void					clear_render(void);
	void					set_texture(SDL_Texture *texture, SDL_Rect rect);
	void					highlight_5inarow(void);
	void					highlight_last_move(int row, int col);
	void					draw_stones(void);
	void					render_buttons(void);
	void					show_stats(void);

	/* Init methods */
	void					load_textures(void);
	void					load_fonts(void);
	void					init_stats(void);

	/* Button methods */
	void					check_buttons(void);
	void					set_buttons(void);
	void					check_buttons_hover(void);
	void					check_buttons_clicked(void);
	void					check_buttons_action(void);

	/* GameAction methods */
	bool					check_action(int action);
	void					set_action(int action);
	void					unset_action(int action);
	void					undo_action(void);

	/* Helper methods */
	void					check_text_hover(void);
	bool					current_is_ai(void) const;
	void					check_game_state(void);
	bool					mouse_on_board(int row, int col) const;
	int						calc_board_placement(int x, int y) const;
	int						get_player_input(void);
	int						get_ai_input(void);
	void					check_ai_clicked(void);
	void					set_ai(int player);
	void					reset_ai(void);
	bool					is_valid_move(int index);
	void					wait_fps(int fps) const;
	void					reset_task(void);
	GuiPlayer				get_winner(void);
	std::string				random_name(void);
	std::string				get_status_update(void);
	SDL_Texture				*load_texture(std::string img_path);
	void					get_hint(void);
	void					reset_hint(void);

	/* Replay methods */
	void					set_replay_settings(std::string board_data_path);
	std::string 			get_board_path(int id) const;
	void		 			load_board_from_id(int id);

	/* Log methods */
	void					clear_log(void);
	void					create_log_dir(void);
	void					log_game_state(void);

	/* Debug methods */
	void					debug(void);
	void					print_duration(void) const;
	void					print_duration(Uint32 start, Uint32 end) const;
};

#endif
