#include "GUI.hpp"

GUI::GUI(IAi *ai, gui_size size) : IGameEngine(ai), update(true), action(def)
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
}

GUI::GUI(IAi *ai) : IGameEngine(ai), update(true), action(def)
{
	this->screen_height = SCREEN_HEIGHT;
	this->screen_width = SCREEN_WIDTH;
	this->size = SIZE;
	this->offset = OFFSET;
	this->interface_size = INTERFACE_SIZE;
	this->btn_size = BUTTON_SIZE;
	this->stats_size = STATS_SIZE;
}

GUI::~GUI()
{
	if (this->btn_font)
		TTF_CloseFont(this->btn_font);
	if (this->stats_font)
		TTF_CloseFont(this->stats_font);
	if (this->board_texture)
    	SDL_DestroyTexture(this->board_texture);
	if (this->p1_texture)
    	SDL_DestroyTexture(this->p1_texture);
	if (this->p2_texture)
    	SDL_DestroyTexture(this->p2_texture);
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

	if (!(this->btn_font = TTF_OpenFont(SCPRO_FONT, this->btn_size))) {
		SDL_Log("Font can't be loaded.");
		return false;
	}

	if (!(this->stats_font = TTF_OpenFont(SCPRO_FONT, this->stats_size))) {
		SDL_Log("Font can't be loaded.");
		return false;
	}

	this->load_textures();
	this->set_buttons();
	this->status = Text(this->renderer, t_point {this->screen_height, (int)this->offset}, this->stats_font);

	return true;
}

void		GUI::gameloop(Board &board)
{
	bool quit = false;
	int index;

    while (!quit)
    {
		index = -1;

		if (this->update)
			this->update_renderer(board);
	
		// SDL_PollEvent(&this->event) --> Use when always want updating, like active animations when no user input
        SDL_WaitEvent(&this->event);

		// if (board.current_player->has_function()) // Check if is AI
		// 	index = board.current_player->fn(board);

		quit = this->handle_events(board, index);

		if (board.place(index))
		{
			board.next_player();
			this->update = true;
		}

		this->check_action(board);
    }
}

void		GUI::check_action(Board &board)
{
	switch (this->action)
	{
	case restart:
		this->reset(board);
		break;
	default:
		this->action = 0;
		break;
	}
}

bool		GUI::handle_events(Board &board, int &index)
{      
	int row, col;
	SDL_GetMouseState(&col, &row);   
	
	switch (this->event.type)
	{
		case SDL_QUIT:
			return true;
		case SDL_MOUSEMOTION:
		{
			for (auto &btn : this->buttons)
			{
				if (btn.on_button(col, row))
					this->update = true;
			}
			break;
		}
		case SDL_MOUSEBUTTONUP:
		{
			if (!board.current_player->has_function() && this->mouse_on_board(row, col) && index == -1)
				index = this->calc_board_placement(row, col);
			else if (!this->mouse_on_board(row, col))
			{
				for (auto &btn : this->buttons)
				{
					if (btn.is_active())
						this->action = btn.get_action();
				}
			}
			break;
		}
	}
	return false;
}

void		GUI::reset(Board &board)
{
	board.reset();
	board.random_player();
	this->update = true;
	this->action = def;
}

void		GUI::draw_stones(Board &board)
{
	SDL_Texture *texture;
	int row, col;

	for (int index = 0; index < board.filled_pos.size(); index++)
	{
		if (board.is_empty_place(index))
			continue;

		texture = board.get_player_id(index) == PLAYER1_ID ? this->p1_texture : this->p2_texture;

		row = (get_row(index) * this->size) + this->offset - (this->size >> 1);
		col = (get_col(index) * this->size) + this->offset - (this->size >> 1);

		this->set_texture(texture, SDL_Rect{col, row, this->size, this->size});
	}
}

void		GUI::set_texture(SDL_Texture *texture, SDL_Rect rect)
{
	SDL_RenderCopy(this->renderer, texture, NULL, &rect);
}

inline bool GUI::mouse_on_board(int row, int col) const { return (row < this->screen_height && col < this->screen_height); }

void		GUI::update_renderer(Board &board)
{
	this->clear_render();
	this->set_texture(this->board_texture, SDL_Rect{0, 0, this->screen_height, this->screen_height});
	this->draw_stones(board);

	this->render_buttons();

	this->update_status(board);

	this->show_stats();

	SDL_RenderPresent(this->renderer);
	this->update = false;
}

void		GUI::clear_render(void)
{
	SDL_Colour colour = BG_COLOUR;

	SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.a);
	SDL_RenderClear(renderer);
}

int			GUI::calc_board_placement(int row, int col) const
{
	row = (row - this->offset) / this->size + .5;
	col = (col - this->offset) / this->size + .5;

	return calc_index(row, col);
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

void		GUI::load_textures(void)
{
    this->board_texture = this->load_texture(BOARD_PATH);
    this->p1_texture = this->load_texture(P1_PATH);
    this->p2_texture = this->load_texture(P2_PATH);
}

void		GUI::set_buttons(void)
{
	this->buttons.push_back(Button(this->renderer, this->screen_height, (int)this->offset << 3, " RESET ", this->btn_font, BG_COLOUR, restart));
	this->buttons.push_back(Button(this->renderer, this->screen_height + (this->interface_size >> 1), (int)this->offset << 3, " PAUSE ", this->btn_font, BG_COLOUR, pause));
	
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
	this->statsP1 = Stats(this->renderer, &board.player1, t_point {this->screen_height, (int)this->offset << 2}, this->stats_font);
	this->statsP2 = Stats(this->renderer, &board.player2, t_point {this->screen_height + (this->interface_size >> 1), (int)this->offset << 2}, this->stats_font);

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

void		GUI::update_status(Board &board)
{
	std::stringstream strm;
	strm << "Current Player: " << board.current_player->name;
	this->status.update(strm.str());
	this->status.render();
}
