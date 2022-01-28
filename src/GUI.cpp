#include "GUI.hpp"

GUI::GUI(IAi *ai, e_gui_size size) : IGameEngine(ai), mouse(t_mouse{.click=false}), winner(NULL), players_playing(2)
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
}

GUI::GUI(IAi *ai) : IGameEngine(ai), mouse(t_mouse{.click=false}), winner(NULL), players_playing(2)
{
	this->screen_height = SCREEN_HEIGHT;
	this->screen_width = SCREEN_WIDTH;
	this->size = SIZE;
	this->offset = OFFSET;
	this->interface_size = INTERFACE_SIZE;
	this->btn_size = BUTTON_SIZE;
	this->stats_size = STATS_SIZE;
	this->status_size = STATUS_SIZE;
}

GUI::~GUI()
{
	for (int i = 0; i < size_font; i++)
	{
		if (this->textures[i])
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

void		GUI::play(Board *board)
{
	if (!this->init("Gomoku"))
        return;
	this->reset(*board);
	this->init_stats(*board);
	this->gameloop(*board);
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
	this->status = Text(this->renderer, t_point {this->screen_height, (int)this->offset}, this->fonts[status_font]);

	return true;
}

void		GUI::gameloop(Board &board)
{
	int index;

    while (!this->check_action(quit))
    {
		if (this->update)
			this->update_renderer(board);
	
		this->handle_events(board);
		
		if (!this->check_action(pause))
		{
			index = this->get_index(board);
			if (board.is_valid_move(index))
			{
				if (!board.current_player->ai)
					this->prev = board;
				board.place(index);
				this->check_game_state(board);
				this->update = true;
			}
		}

		this->check_actions(board);
    }
}

void		GUI::update_renderer(Board &board)
{
	this->clear_render();
	this->set_texture(this->textures[board_tex], SDL_Rect{0, 0, this->screen_height, this->screen_height});
	
	this->draw_stones(board);
	this->render_buttons();

	this->status.update(this->get_status_update(board));
	this->status.render();

	this->show_stats();

	SDL_RenderPresent(this->renderer);
	this->update = false;
}

void		GUI::check_actions(Board &board)
{
	if (this->check_action(restart))
		this->reset(board);
	else if (this->check_action(undo))
		this->undo_action(board);
}

void		GUI::undo_action(Board &board)
{
	board = this->prev;
	this->update = true;
	this->unset_action(undo);
	if (this->winner)
	{
		this->winner = NULL;
		this->unset_action(pause);
	}
}

void		GUI::handle_events(Board &board)
{
	// SDL_PollEvent(&this->event); //--> Use when always want updating, like active animations when no user input
	SDL_WaitEvent(&this->event);
	SDL_GetMouseState(&this->mouse.pos.x, &this->mouse.pos.y);   
	this->mouse.click = false;

	switch (this->event.type)
	{
		case SDL_QUIT:
			this->set_action(quit);
			break;
		case SDL_MOUSEMOTION:
		{
			for (auto &btn : this->buttons)
			{
				if (btn.on_button(this->mouse.pos.x, this->mouse.pos.y))
					this->update = true;
			}
			break;
		}
		case SDL_MOUSEBUTTONUP:
		{
			this->mouse.click = true;
			for (auto &btn : this->buttons)
			{
				if (btn.is_active())
					this->set_action(btn.get_action());
			}
			break;
		}
	}
}

int			GUI::get_player_input(void)
{
	if (this->mouse.click && this->mouse_on_board(this->mouse.pos.x, this->mouse.pos.y))
		return this->calc_board_placement(this->mouse.pos.x, this->mouse.pos.y);
	return -1;
}

void		GUI::check_game_state(Board &board)
{
	if (board.is_game_finished(*board.current_player))
	{
		this->set_action(pause);
		this->winner = board.get_player_by_id(board.winner);
	}
	else
		board.next_player();
}

std::string	GUI::get_status_update(Board &board) const
{
	if (this->winner)
		return this->winner->name + " Won";
	else if (board.is_full())
		return "Draw";
	else
		return "> " + board.current_player->name;
}

void		GUI::reset(Board &board)
{
	board.reset();
	board.random_player();
	while ( (board.player1.name = this->random_name()).length() > 16);
	while ( (board.player2.name = this->random_name()).length() > 16);
	this->set_players_ai(board);
	this->update = true;
	this->action = def;
	this->winner = NULL;
	this->prev = board;
}

void		GUI::draw_stones(Board &board)
{
	SDL_Texture *texture;
	int row, col, index;

	for (index = 0; index < board.filled_pos.size(); index++)
	{
		if (board.is_empty_place(index))
			continue;

		texture = board.get_player_id(index) == PLAYER1_ID ? this->textures[p1_tex] : this->textures[p2_tex];
		row = (misc::get_row(index) * this->size) + this->offset - (this->size >> 1);
		col = (misc::get_col(index) * this->size) + this->offset - (this->size >> 1);

		this->set_texture(texture, SDL_Rect{col, row, this->size, this->size});
		if (index == board.get_last_move())
			this->highlight_last_move(board, row, col);

	}
	if (this->winner && this->winner->has_wincondition())
		this->highlight_5inarow(board);
}

void		GUI::highlight_last_move(Board &board, int row, int col)
{
	SDL_Texture *texture = board.get_player_id(board.get_last_move()) == PLAYER1_ID ? this->textures[p1_select_tex] : this->textures[p2_select_tex];
	this->set_texture(texture, SDL_Rect{col, row, this->size, this->size});
}

void		GUI::highlight_5inarow(Board &board)
{
	Heuristic heuristic;

	int index = this->winner->winning_index;
	int dir = heuristic.check_wincodition_all_dir(&board, index, this->winner->id);
	index += (dir * heuristic.count_direction(&board, index, this->winner->id, dir, 10));
	dir = -dir;

	int prev_index, row, col;
	while (board.get_player_id(index) == this->winner->id)
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
	this->fonts[btn_font] = TTF_OpenFont(SCPRO_FONT, this->btn_size);
	this->fonts[stats_font]= TTF_OpenFont(SCPRO_FONT, this->stats_size);
	this->fonts[status_font] = TTF_OpenFont(SANS_FONT, this->status_size);
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
	int w = this->screen_height;
	int h = this->offset + (this->size * (3 + (size_texts >> 1)));

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

void		GUI::init_stats(Board &board)
{
	this->statsP1 = Stats(this->renderer, &board.player1, t_point {this->screen_height, (int)this->offset << 2}, this->fonts[stats_font]);
	this->statsP2 = Stats(this->renderer, &board.player2, t_point {this->screen_height + (this->interface_size >> 1), (int)this->offset << 2}, this->fonts[stats_font]);

	this->statsP1.init();
	this->statsP2.init();
}

void		GUI::show_stats(void)
{
	this->statsP1.update();
	this->statsP2.update();
	
	this->statsP1.render();
	this->statsP2.render();
}

int			GUI::get_index(Board &board)
{
	if (board.current_player->ai)
		return board.current_player->ai->calculate(board);
	else
		return this->get_player_input();
}

bool		GUI::check_action(int action)
{
	return ((this->action & action) == action);
}

void		GUI::set_action(int action)
{
	this->action = this->action | action;
}

void		GUI::unset_action(int action)
{
	this->action = this->action ^ action;
}

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
        name += tokens[misc::random(0, tokens.size() - 1)];
    }
    nameFileout.close();
    return name;
}

void		GUI::set_players_ai(Board &board)
{
	switch (this->players_playing)
	{
	case 0:
		board.player1.ai = this->ai;
		board.player2.ai = this->ai;
		break;
	case 1:
		board.player1.ai = NULL;
		board.player2.ai = this->ai;
		break;
	case 2:
		board.player1.ai = NULL;
		board.player2.ai = NULL;
		break;
	default:
		break;
	}
}