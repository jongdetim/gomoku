#ifndef GUI_HPP
# define GUI_HPP

# include <SDL.h>
# include <SDL_ttf.h>
# include "IGameEngine.hpp"
# include "Board.hpp"
# include "Button.hpp"
# include "Stats.hpp"
# include "GuiBoard.hpp"

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

# define SANS_FONT "resources/fonts/OpenSans/Regular.ttf"
# define SANS_FONT_BOLD "resources/fonts/OpenSans/Bold.ttf"
# define SANS_FONT_XTRABOLD "resources/fonts/OpenSans/ExtraBold.ttf"
# define SANS_FONT_SEMIBOLD "resources/fonts/OpenSans/SemiBold.ttf"

# define SCPRO_FONT "resources/fonts/SourceCodePro/Regular.ttf"

# define ALLER_FONT "resources/fonts/Aller/Regular.ttf"
# define ALLER_FONT_BOLD "resources/fonts/Aller/Bold.ttf"
# define ALLER_FONT_DISPLAY "resources/fonts/Aller/Display.ttf"

# define POPPINS_FONT "resources/fonts/Poppins/Regular.otf"
# define POPPINS_FONT_BOLD "resources/fonts/Poppins/Bold.otf"
# define POPPINS_FONT_XTRABOLD "resources/fonts/Poppins/ExtraBold.otf"
# define POPPINS_FONT_SEMIBOLD "resources/fonts/Poppins/SemiBold.otf"
# define POPPINS_FONT_MED "resources/fonts/Poppins/Medium.otf"


# define BTN_FONT ALLER_FONT
# define STATS_FONT SANS_FONT
# define STATS_NAME_FONT SANS_FONT_BOLD
# define STATUS_FONT POPPINS_FONT_BOLD
# define TITLE_FONT "resources/fonts/Title.ttf"

enum e_fonts
{
	btn_font,
	stats_font,
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
	bool 		click;
}				t_mouse;

class GUI: public IGameEngine
{
public:
	GUI(IAi *ai);
	GUI(IAi *ai, e_gui_size size);
	~GUI();

	void					play(Board *board);

private:
	GuiBoard				guiboard;

	int						screen_height;
	int						screen_width;
	int						size;
	int						interface_size;
	double					offset;
	int						btn_size;
	int						stats_size;
	int						status_size;
	int						title_size;
	
	SDL_Window				*window;
	SDL_Renderer			*renderer;
	SDL_Event				event;

	TTF_Font				*fonts[size_font];
	SDL_Texture				*textures[size_tex];
	Stats					stats[2];

	std::vector<Button>		buttons;
	Text					title;
	Text					status;
	t_mouse					mouse;
	GuiBoard				prev;
	short					players_playing;

	bool					update;
	int						action;

	bool					init(std::string title);
	void					gameloop(void);
	void					check_game_state(void);
	void					reset(void);
	bool					mouse_on_board(int row, int col) const;
	void					clear_render(void);
	void					set_texture(SDL_Texture *texture, SDL_Rect rect);
	void					draw_stones(void);
	void					highlight_5inarow(void);
	void					highlight_last_move(int row, int col);
	void					update_renderer(void);
	void					handle_events(void);
	int						calc_board_placement(int x, int y) const;
	SDL_Texture				*load_texture(std::string img_path);
	void					load_textures(void);
	void					load_fonts(void);
	void					set_buttons(void);
	void					render_buttons(void);
	void					init_stats(void);
	void					show_stats(void);
	int						get_index(void);
	int						get_player_input(void);
	bool					check_action(int action);
	void					set_action(int action);
	void					unset_action(int action);
	std::string				get_status_update(void);
	void					check_actions(void);
	void					undo_action(void);
	std::string				random_name(void);
	GuiPlayer				get_winner(void);
	void					set_ai(void);
};

#endif
