#include "GUI.hpp"
#include <SDL_image.h>
#include "misc.hpp"
#include "heuristic.hpp"

GUI::GUI(IAi *ai, e_gui_size size) : IGameEngine(ai), mouse(t_mouse{.clicked=false}), fonts{0}, textures{0}, ticks(0), replay_mode(false)
{
	int height;

	switch (size)
	{
	case big:
		height = SCREEN_HEIGHT;
		break;
	case medium:
		height = 768;
		break;
	case small:
		height = 576;
		break;
	default:
		height = SCREEN_HEIGHT;
		break;
	}

	this->screen_height = height;
	this->interface_size = (height * INTERFACE_SIZE / (double)SCREEN_HEIGHT) + .5;
	this->screen_width = height + this->interface_size;
	this->size = (height * SIZE / (double)SCREEN_HEIGHT) + .5;
	this->offset = height * OFFSET / (double)SCREEN_HEIGHT;
	this->btn_size = height * BUTTON_SIZE / (double)SCREEN_HEIGHT;
	this->stats_size = height * STATS_SIZE / (double)SCREEN_HEIGHT;
	this->status_size = height * STATUS_SIZE / (double)SCREEN_HEIGHT;
	this->title_size = height * TITLE_SIZE / (double)SCREEN_HEIGHT;
}

GUI::GUI(void) : GUI(NULL, big) {}

GUI::GUI(IAi *ai) : GUI(ai, big) {}

GUI::~GUI()
{
	for (int i = 0; i < size_font; i++)
	{
		if (this->fonts[i])
			TTF_CloseFont(this->fonts[i]);
	}
	for (int i = 0; i < size_tex; i++)
	{
		if (this->textures[i])
			SDL_DestroyTexture(this->textures[i]);
	}
    
	if (this->renderer)
		SDL_DestroyRenderer(this->renderer);
	if (this->window)
    	SDL_DestroyWindow(this->window);
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

void		GUI::play(Board board)
{
	if (!this->init("Gomoku"))
        return;
	this->guiboard = GuiBoard(board);
	this->init_game();
	this->init_stats();
	this->gameloop();
}

void		GUI::replay(std::string board_data_path)
{
	int index;
	Board board;
	std::string file_name;
	
	index = board_data_path.find_last_of("/") + 1;
	this->replay_mode = true;
	this->dir = board_data_path.substr(0, index);
	file_name = board_data_path.substr(index, board_data_path.length());
	if (!isdigit(file_name[0]))
		throw "incorrect file name id";
	this->current_id = this->starting_id = atoi(file_name.c_str());
	board.load(board_data_path);
	this->play(board);
}

/* Private Methods */
bool		GUI::init(std::string title)
{

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
	}
	
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();

	SDL_CreateWindowAndRenderer(this->screen_width, this->screen_height, SDL_WINDOW_SHOWN, &this->window, &this->renderer);

	if (window == NULL) {
        SDL_Log("Unable to create window: %s", SDL_GetError());
        return false;
    }

	this->load_fonts();
	this->load_textures();
	
	this->set_buttons();
	
	this->status = Text(this->renderer, t_point {this->screen_height, ((int)this->offset << 1) + (this->size << 1)});
	this->title = Text(this->renderer, t_point {this->screen_height, (int)this->offset});
	this->title.update("* GOMOKU *", this->fonts[title_font]);

	return true;
}

void		GUI::gameloop(void)
{
	int index;

    while (!this->check_action(quit))
    {
		this->ticks = SDL_GetTicks();
	
		this->handle_events();
		
		if (!this->check_action(pauze))
		{
			index = this->get_index();
			if (this->is_valid_move(index))
			{
				if (!this->guiboard.current_player().ai)
					this->prev = this->guiboard;
				GUIBOARD.place(index);
				this->check_game_state();

				// this->debug();
				this->log_game_state();
			}
		}

		if (this->mouse.clicked)
		{
			this->check_text_clicked();
			this->check_buttons_clicked();
			this->check_buttons_action();
		}

		if (this->update)
			this->update_renderer();

		this->wait_fps(FPS);
    }
}

void		GUI::handle_events(void)
{
	SDL_Event event;
	SDL_PollEvent(&event);
	this->mouse.clicked = false;

	switch (event.type)
	{
		case SDL_QUIT:
			this->set_action(quit);
			break;
		case SDL_KEYDOWN:
		{
			this->key_press(event.key.keysym.sym);
			break;
		}
		case SDL_MOUSEMOTION:
		{
			SDL_GetMouseState(&this->mouse.pos.x, &this->mouse.pos.y);
			this->check_buttons_hover();
			break;
		}
		case SDL_MOUSEBUTTONDOWN:
		{
			this->mouse.clicked = true;
			SDL_FlushEvent(SDL_MOUSEBUTTONDOWN);
			break;
		}
	}
}

std::string GUI::get_board_path(int id) const { return this->dir + std::to_string(id) + (std::string)BOARD_DATA_FILE_EXT; }

void		GUI::load_board_from_id(int id)
{
	std::string board_path = this->get_board_path(id);
	try
	{
		GUIBOARD.load(board_path);
		this->current_id = id;
		this->update = true;
		if (GUIBOARD.has_winner())
			this->set_action(pauze);
		else if (this->check_action(pauze))
			this->unset_action(pauze);
	}
	catch(const char *e) {}
}

bool		GUI::is_valid_move(int index)
{
	int player = GUIBOARD.get_current_player();
	GUIBOARD.last_move_was_capture = GUIBOARD.is_capture(player, index);
	return (GUIBOARD.is_valid_move(index) && !GUIBOARD.is_free_threes(index, player));
}

void		GUI::update_renderer(void)
{
	this->clear_render();
	this->set_texture(this->textures[board_tex], SDL_Rect{0, 0, this->screen_height, this->screen_height});
	
	this->draw_stones();
	this->render_buttons();

	this->status.update(this->get_status_update(), this->fonts[status_font]); // Find better place for this
	this->status.render();
	this->title.render();

	this->show_stats();

	SDL_RenderPresent(this->renderer);
	this->update = false;
}

void		GUI::check_buttons_hover(void)
{
	for (auto &btn : this->buttons)
	{
		if (btn.on_button(this->mouse.pos.x, this->mouse.pos.y))
			this->update = true;
	}
}

void		GUI::check_buttons_clicked(void)
{
	if (!this->mouse.clicked)
		return;
	for (auto &btn : this->buttons)
	{
		if (btn.is_active())
		{
			this->set_action(btn.get_action());
			break;
		}
	}
}

void		GUI::check_buttons_action(void)
{
	if (this->check_action(restart))
		this->reset();
	else if (this->check_action(undo))
		this->undo_action();
}

void		GUI::check_text_clicked(void)
{
	if (!this->mouse.clicked)
		return;
	for (int i = 0; i < 2; i++)
	{
		if (this->stats[i].on_text(this->mouse.pos.x, this->mouse.pos.y, player_text))
		{
			this->set_ai(i);
			break;
		}
	}
}

void		GUI::undo_action(void)
{
	if (GUIBOARD.has_winner())
		this->unset_action(pauze);
	this->guiboard = this->prev;
	this->update = true;
	this->unset_action(undo);

	this->log_game_state();
	// this->debug();
}

int			GUI::get_player_input(void)
{
	if (this->mouse.clicked && this->mouse_on_board(this->mouse.pos.x, this->mouse.pos.y))
		return this->calc_board_placement(this->mouse.pos.x, this->mouse.pos.y);
	return -1;
}

void		GUI::check_game_state(void)
{
	if (GUIBOARD.is_game_finished())
		this->set_action(pauze);
	else
		GUIBOARD.next_player();
	this->update = true;
}

std::string	GUI::get_status_update(void)
{
	if (GUIBOARD.has_winner())
		return this->get_winner().name + " Won";
	else if (GUIBOARD.is_full())
		return "Draw";
	else
		return "> " + this->guiboard.current_player().name;
}

void		GUI::init_game(void)
{
	while ( (this->guiboard.players[PLAYER1].name = random_name()).length() > 14);
	while ( (this->guiboard.players[PLAYER2].name = random_name()).length() > 14);
	this->reset_ai();
	this->prev = this->guiboard;
	this->action = def;
	this->update = true;
	
	this->clear_log();
	this->log_game_state();
	// this->debug();
}

void		GUI::reset(void)
{
	if (this->replay_mode)
	{
		this->load_board_from_id(this->starting_id);
		this->action = def;
		this->reset_ai();
	}
	else
	{
		GUIBOARD.reset();
		GUIBOARD.random_player();
		this->init_game();
	}
}

void		GUI::draw_stones(void)
{
	SDL_Texture *texture;
	int row, col, index;

	for (index = 0; index < GUIBOARD.filled_pos.size(); index++)
	{
		if (GUIBOARD.is_empty_place(index))
			continue;

		texture = GUIBOARD.get_player(index) == PLAYER1 ? this->textures[p1_tex] : this->textures[p2_tex];
		row = (misc::get_row(index) * this->size) + this->offset - (this->size >> 1);
		col = (misc::get_col(index) * this->size) + this->offset - (this->size >> 1);

		this->set_texture(texture, SDL_Rect{col, row, this->size, this->size});
		if (index == GUIBOARD.get_last_move())
			this->highlight_last_move(row, col);

	}
	if (GUIBOARD.has_winner() && this->get_winner().wincondition())
		this->highlight_5inarow();
}

void		GUI::highlight_last_move(int row, int col)
{
	SDL_Texture *texture = GUIBOARD.get_player(GUIBOARD.get_last_move()) == PLAYER1 ? this->textures[p1_select_tex] : this->textures[p2_select_tex];
	this->set_texture(texture, SDL_Rect{col, row, this->size, this->size});
}

void		GUI::highlight_5inarow(void)
{
	int player = GUIBOARD.winner;
	int index = this->get_winner().last_move();
	int dir = GUIBOARD.check_wincondition_all_dir(index, player);
	index += (dir * heuristic::count_direction(this->guiboard.get_board(), index, player, dir, BOARD_LENGTH));
	dir = -dir;

	int prev_index, row, col;
	while (GUIBOARD.get_player(index) == player)
	{
		row = (misc::get_row(index) * this->size) + this->offset - (this->size >> 1);
		col = (misc::get_col(index) * this->size) + this->offset - (this->size >> 1);
		this->set_texture(this->textures[winning_tex], SDL_Rect{col, row, this->size, this->size});

		prev_index = index;
		index += dir;
		if (misc::is_offside(prev_index, index))
			break;
	}
}

void		GUI::set_texture(SDL_Texture *texture, SDL_Rect rect)
{
	SDL_RenderCopy(this->renderer, texture, NULL, &rect);
}

void		GUI::key_press(int key)
{
	switch (key)
	{
		case SDLK_LEFT:
		case SDLK_RIGHT:
		{
			if (this->replay_mode)
			{
				int id = key == SDLK_RIGHT ? this->current_id + 1 : this->current_id - 1;
				this->load_board_from_id(id);
				this->reset_ai();
			}
			break;
		}
	}
}

inline bool GUI::mouse_on_board(int x, int y) const { return (y < this->screen_height && x < this->screen_height); }

void		GUI::clear_render(void)
{
	SDL_Colour colour = BG_COLOUR;

	SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.a);
	SDL_RenderClear(renderer);
}

int			GUI::calc_board_placement(int x, int y) const
{
	int row = (y - this->offset) / this->size + .5;
	int col = (x - this->offset) / this->size + .5;

	return misc::calc_index(row, col);
}

SDL_Texture	*GUI::load_texture(std::string img_path)
{
	SDL_Surface	*image;
	SDL_Texture	*texture;

	image = IMG_Load(img_path.c_str());
	texture = SDL_CreateTextureFromSurface(this->renderer, image);
	
	if (image)
	    SDL_FreeSurface(image);

	return texture;
}

void		GUI::load_fonts(void)
{
	this->fonts[btn_font] = TTF_OpenFont(BTN_FONT, this->btn_size);

	this->fonts[stats_font]= TTF_OpenFont(STATS_FONT, this->stats_size);
	this->fonts[stats_name_font] = TTF_OpenFont(STATS_NAME_FONT, this->stats_size);
	
	this->fonts[status_font] = TTF_OpenFont(STATUS_FONT, this->status_size);
	this->fonts[title_font] = TTF_OpenFont(TITLE_FONT, this->title_size);
}

void		GUI::load_textures(void)
{
	this->textures[board_tex] = this->load_texture(BOARD_PATH);
    this->textures[p1_tex] = this->load_texture(P1_PATH);
    this->textures[p2_tex] = this->load_texture(P2_PATH);
    this->textures[p1_select_tex] = this->load_texture(P1_SELECT);
    this->textures[p2_select_tex] = this->load_texture(P2_SELECT);
    this->textures[winning_tex] = this->load_texture(WIN_SELECT);
}

void		GUI::set_buttons(void)
{
	int btn_w = 0;
	int w = this->screen_height + this->offset;
	int h = this->offset + (this->size * 17);

	this->buttons.push_back(
		Button(this->renderer, w + btn_w, h, " UNDO ", this->fonts[btn_font], BG_COLOUR, undo));
	btn_w += this->buttons[0].get_button_size().x + this->offset;

	this->buttons.push_back(
		Button(this->renderer, w + btn_w, h, " RESTART ", this->fonts[btn_font], BG_COLOUR, restart));
	btn_w += this->buttons[1].get_button_size().x + this->offset;

	this->buttons.push_back(
		Button(this->renderer, w + btn_w, h, " QUIT ", this->fonts[btn_font], BG_COLOUR, quit));

	for (auto &btn : this->buttons)
		btn.init();
}

void		GUI::render_buttons(void)
{
	for (auto &btn : this->buttons)
		btn.render();
}

void		GUI::init_stats(void)
{
	this->stats[PLAYER1] = Stats(this->renderer, t_point {this->screen_height, (int)this->offset + (this->size << 2)}, this->fonts[stats_font], this->fonts[stats_name_font]);
	this->stats[PLAYER2] = Stats(this->renderer, t_point {this->screen_height + (this->interface_size >> 1), (int)this->offset + (this->size << 2)}, this->fonts[stats_font], this->fonts[stats_name_font]);

	this->stats[PLAYER1].init();
	this->stats[PLAYER2].init();
}

void		GUI::show_stats(void)
{
	this->stats[PLAYER1].update(this->guiboard.players[PLAYER1]);
	this->stats[PLAYER2].update(this->guiboard.players[PLAYER2]);
	
	this->stats[PLAYER1].render();
	this->stats[PLAYER2].render();

	for (int player = 0; player < 2; player++)
	{
		t_point pos = this->stats[player].get_position(captures_text);
		SDL_Texture *texture = player == PLAYER1 ? this->textures[p1_tex] : this->textures[p2_tex];
		this->set_texture(texture, SDL_Rect{ pos.x , pos.y + this->size, (this->size>>1), (this->size>>1)});
	}
}

int			GUI::get_index(void)
{
	if (this->guiboard.current_player().ai)
	{
		int index = this->guiboard.current_player().ai->calculate(this->guiboard.get_board());
<<<<<<< HEAD
		if (index < 0 || index >= BOARDSIZE)
			PRINT("GUI: Invalid index: " + std::to_string(index));
		return this->guiboard.current_player().ai->calculate(this->guiboard.get_board());
=======
		assert(index >= 0 && index < BOARDSIZE);
		return index;
>>>>>>> gui
	}
	else
		return get_player_input();
}

bool		GUI::check_action(int action) { return ((this->action & action) == action); }

void		GUI::set_action(int action) { this->action = this->action | action; }

void		GUI::unset_action(int action) { this->action = this->action ^ action; }

std::string	GUI::random_name(void)
{
	std::string value, name("");
    std::ifstream nameFileout;
    std::vector<std::string> tokens;

    nameFileout.open(NAMES);
    while (std::getline(nameFileout, value))
    {
        if (value.empty())
            continue;
        tokens = misc::tokenize(value, ',');
        name += tokens[misc::get_random_int_range(0, tokens.size() - 1)];
    }
    nameFileout.close();
    return name;
}

GuiPlayer	GUI::get_winner(void) { return this->guiboard.players[GUIBOARD.winner]; }

void		GUI::reset_ai(void)
{
	for (int i = 0; i < 2; i++)
		this->guiboard.players[i].ai = NULL;
}

void		GUI::set_ai(int player)
{
	if (this->guiboard.players[player].ai)
		this->guiboard.players[player].ai = NULL;
	else
		this->guiboard.players[player].ai = this->ai;
	this->update = true;
}

void		GUI::clear_log(void)
{
	system("rm -rf log/");
	this->create_log_dir();
}

void		GUI::create_log_dir(void) { mkdir("log", 0777); }

void		GUI::log_game_state(void)
{
    std::ofstream log;
	static int id = 1;

    log = std::ofstream("log/log.txt", std::ios::app);

	log << "ID[" << id << "] Captures P1: " << GUIBOARD.players[PLAYER1].captures << "\t\t\t\tP2: " << GUIBOARD.players[PLAYER2].captures << std::endl;
	log << "   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8" << std::endl;
	for (int row = 0; row < BOARD_LENGTH; row++)
	{
		log << row%10 << ": ";
		for (int col = 0; col < BOARD_LENGTH; col++)
		{
			int index = (row * BOARD_LENGTH + col);
			if (GUIBOARD.is_empty_place(index))
				log << ". ";
			else if (GUIBOARD.get_state()[index<<1])
			{
				if (GUIBOARD.get_last_move() == index)
					log << 0 << ' ';
				else
					log << P1_SYMBOL << ' ';
			}
			else
			{
				if (GUIBOARD.get_last_move() == index)
					log << (char)(P2_SYMBOL-32) << ' ';
				else
					log << P2_SYMBOL << ' ';
			}
		}
		log << std::endl;
	}
	log << std::endl;
    log.close();
	GUIBOARD.save("log/" + std::to_string(id) + BOARD_DATA_FILE_EXT);
	id++;
}

void		GUI::debug(void)
{
	system("clear");
	if (GUIBOARD.get_last_move() != -1)
    	GUIBOARD.h = heuristic::get_heuristic_total(GUIBOARD, GUIBOARD.get_last_player());
    GUIBOARD.print_values();
	PRINT("");
}

void		GUI::wait_fps(int fps) const
{
	int ms = 1000.0f / fps + .5;
	int delay = ms - (SDL_GetTicks() - this->ticks);
	if (delay > 0)
		SDL_Delay(delay);

	// Uint32 end = SDL_GetTicks();
	// float secondsElapsed = (end - this->ticks) / 1000.0f;
	// PRINT("Seconds: " << secondsElapsed);
}
