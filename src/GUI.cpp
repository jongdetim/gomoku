#include "GUI.hpp"
#include "misc.hpp"

GUI::GUI(gui_size size) : update(true)
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
}

GUI::GUI(void) : update(true)
{
	this->screen_height = SCREEN_HEIGHT;
	this->screen_width = SCREEN_WIDTH;
	this->size = SIZE;
	this->offset = OFFSET;
	this->interface_size = INTERFACE_SIZE;
}

GUI::~GUI()
{
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

bool		GUI::initiate_GUI(std::string title)
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

    this->board_texture = this->load_texture(BOARD_PATH);
    this->p1_texture = this->load_texture(P1_PATH);
    this->p2_texture = this->load_texture(P2_PATH);

	// this->initiate_buttons();

	return true;
}

void		GUI::game(Board &board)
{
	bool quit = false;
	int index;
    
	this->reset(board);

    while (!quit)
    {
		index = -1;

		if (this->update)
			this->update_renderer(board);
	
		// SDL_PollEvent(&this->event) --> Use when always want updating, like active animations when no user input
        SDL_WaitEvent(&this->event);

		if (board.current_player->has_function())
			index = board.current_player->fn(board);

		quit = this->handle_events(board, index);

		if (board.place(index))
		{
			board.next_player();
			this->update = true;
		}

		// if (board.is_game_finished(*board.current_player))
		// {
		// 	std::string input;

		// 	std::getline(std::cin, input);
		// 	break;
		// }
    }
}

/* Private Methods */

bool		GUI::handle_events(Board &board, int &index)
{         
	switch (this->event.type)
	{
		case SDL_QUIT:
			return true;
		case SDL_MOUSEBUTTONUP:
		{
			int row, col;
			SDL_GetMouseState(&col, &row);
			
			if (!board.current_player->has_function() && this->mouse_on_board(row, col))
				index = this->calc_board_placement(row, col);
			else if (!this->mouse_on_board(row, col))
			{
				;
			}

			break;
		}
	}
	return false;
}

void		GUI::draw_interface(Board &board)
{
	// auto col = this->subtract_colour(SDL_Colour BG_COLOUR, 20);

	// for (auto btn : this->buttons)
	// 	this->place_button(btn.col, /*SDL_Colour BG_COLOUR,*/ btn.rec);
}

void		GUI::reset(Board &board)
{
	board.reset();
	board.random_player();
	this->update = true;
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

inline bool GUI::mouse_on_board(int row, int col) { return (row < this->screen_height && col < this->screen_height); }

void		GUI::update_renderer(Board &board)
{
	this->clear_render();
	this->set_texture(this->board_texture, SDL_Rect{0, 0, this->screen_height, this->screen_height});
	this->draw_stones(board);
	this->draw_interface(board);

	SDL_RenderPresent(this->renderer);
	this->update = false;
}

void		GUI::clear_render(void)
{
	SDL_Colour colour BG_COLOUR;

	SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.a);
	SDL_RenderClear(renderer);
}

int			GUI::calc_board_placement(int row, int col)
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

SDL_Colour	GUI::subtract_colour(SDL_Colour col, int amount) const
{
	SDL_Colour colour;

	colour.r = col.r - amount;
	colour.g = col.g - amount;
	colour.b = col.b - amount;
	colour.a = 255;

	return colour;
}

inline bool	GUI::in_square(int x, int y, SDL_Rect rec)
{
	return (x > rec.x && x <= (rec.x + rec.w)) && (y > rec.y && y <= (rec.y + rec.h));
}

// void		GUI::place_button(SDL_Colour col, SDL_Rect rect)
// {
// 	SDL_SetRenderDrawColor(renderer, col.r, col.g, col.b, col.a);
	// SDL_RenderFillRect(this->renderer, &rect); 
	// SDL_RenderDrawRect(); 
// }

// void		GUI::initiate_buttons(void)
// {
// 	t_button button {
// 		SDL_Rect { this->screen_height + (int)this->offset, (int)this->offset, this->interface_size - ((int)this->offset << 1), this->size << 1 },
// 		this->subtract_colour(SDL_Colour BG_COLOUR, 20)
// 	};
// 	this->buttons.push_back(button);


// SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
// SDL_Rect rec BUTTON_LEFT (this->screen_height + (int)this->offset, (int)this->offset);
// SDL_RenderDrawRect(this->renderer, &rec);

// SDL_Rect rec2 BUTTON_RIGHT(this->screen_height + (int)this->offset, (int)this->offset);
// SDL_RenderDrawRect(this->renderer, &rec2);

// SDL_Rect rec3 BUTTON_BIG(this->screen_height + (int)this->offset, (int)this->offset + (this->size * 3));
// SDL_RenderDrawRect(this->renderer, &rec3);

// }