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
	SDL_Quit();
}

bool		GUI::initiate_GUI(std::string title)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
	}

	SDL_CreateWindowAndRenderer(this->screen_width, this->screen_height, SDL_WINDOW_SHOWN, &this->window, &this->renderer);

	if (window == NULL) {
        SDL_Log("Unable to create window: %s", SDL_GetError());
        return false;
    }

    this->board_texture = this->load_texture(BOARD_PATH);
    this->p1_texture = this->load_texture(P1_PATH);
    this->p2_texture = this->load_texture(P2_PATH);

	return true;
}

void		GUI::game(Board &board)
{
	bool quit = false;
    
    while (!quit)
    {
		if (this->update)
			this->update_renderer(board);
	
        SDL_WaitEvent(&this->event); // SDL_PollEvent(&this->event) --> Use when always want updating, like active animations when no user input

		quit = this->handle_events(board);
		
    }
}

/* Private Methods */

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

bool		GUI::handle_events(Board &board)
{         
	switch (this->event.type)
	{
		case SDL_QUIT:
			return true;
		case SDL_MOUSEBUTTONUP:
		{
			int row, col;
			SDL_GetMouseState(&col, &row);
			
			if (this->mouse_on_board(row, col))
				this->place_on_board(board, row, col);
			// else
			// 	click on info

			break;
		}
	}
	return false;
}

void		GUI::update_renderer(Board &board)
{
	this->clear_render();
	this->set_texture(this->board_texture, SDL_Rect{0, 0, this->screen_height, this->screen_height});
	this->draw_stones(board);
	SDL_RenderPresent(this->renderer);
	this->update = false;
}

void		GUI::clear_render(void)
{
	SDL_SetRenderDrawColor(renderer, 220, 179, 92, 255);
	SDL_RenderClear(renderer);
}

void		GUI::place_on_board(Board &board, int row, int col)
{
	row = (row - this->offset) / this->size + .5;
	col = (col - this->offset) / this->size + .5;

	if (board.place(calc_index(row, col)))
	{
		board.next_player();
		this->update = true;
	}
}

SDL_Texture	*GUI::load_texture(std::string img_path)
{
	SDL_Surface	*image;
	SDL_Texture	*texture;

	image = SDL_LoadBMP(img_path.c_str());
	texture = SDL_CreateTextureFromSurface(this->renderer, image);
	
	if (image)
	    SDL_FreeSurface(image);

	return texture;
}
