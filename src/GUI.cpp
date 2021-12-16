#include "GUI.hpp"
#include "misc.hpp"

GUI::GUI(void) : update(true) { }

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

	SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN, &this->window, &this->renderer);

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

		row = get_row(index);
		col = get_col(index);

		row = (row * SIZE) + OFFSET - (SIZE >> 1);
		col = (col * SIZE) + OFFSET - (SIZE >> 1);

		this->set_texture(texture, SDL_Rect{col, row, SIZE, SIZE});
	}
}

void		GUI::set_texture(SDL_Texture *texture, SDL_Rect rect)
{
	SDL_RenderCopy(this->renderer, texture, NULL, &rect);
}

bool		GUI::handle_events(Board &board)
{         
	switch (this->event.type)
	{
		case SDL_QUIT:
			return true;
		case SDL_MOUSEBUTTONUP:
		{
			int row, col;
			
			this->get_placement(&row, &col);

			if (board.place(calc_index(row, col)))
			{
				board.next_player();
				this->update = true;
			}

			break;
		}
	}
	return false;
}

void		GUI::update_renderer(Board &board)
{
	this->clear_render();
	this->set_texture(this->board_texture, SDL_Rect{0, 0, SCREEN_HEIGHT, SCREEN_HEIGHT});
	this->draw_stones(board);
	SDL_RenderPresent(this->renderer);
	this->update = false;
}

void		GUI::clear_render(void)
{
	SDL_SetRenderDrawColor(renderer, 220, 179, 92, 255);
	SDL_RenderClear(renderer);
}

void		GUI::get_placement(int *row, int *col)
{
	SDL_GetMouseState(col, row);
	
	*row = (*row - OFFSET) / (double)SIZE + .5;
	*col = (*col - OFFSET) / (double)SIZE + .5;
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
