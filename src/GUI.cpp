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
	this->btn_size = height * BTN_SIZE / (double)SCREEN_HEIGHT;
}

GUI::GUI(void) : update(true)
{
	this->screen_height = SCREEN_HEIGHT;
	this->screen_width = SCREEN_WIDTH;
	this->size = SIZE;
	this->offset = OFFSET;
	this->interface_size = INTERFACE_SIZE;
	this->btn_size = BTN_SIZE;
}

GUI::~GUI()
{
	this->free_buttons();

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

	this->load_textures();
	this->set_buttons();

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

		if (board.current_player->has_function()) // Check if is AI
			index = board.current_player->fn(board);

		quit = this->handle_events(board, index);

		if (board.place(index))
		{
			board.next_player();
			this->update = true;
		}
    }
}

/* Private Methods */

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
				if (btn.is_active(col, row))
				{
					this->update = true;
					break;
				}
			}
			break;
		}
		case SDL_MOUSEBUTTONUP:
		{
			if (!board.current_player->has_function() && this->mouse_on_board(row, col) && index == -1)
				index = this->calc_board_placement(row, col);
			else if (!this->mouse_on_board(row, col))
				;
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

	this->render_buttons();

	SDL_RenderPresent(this->renderer);
	this->update = false;
}

void		GUI::clear_render(void)
{
	SDL_Colour colour = BG_COLOUR;

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

void		GUI::load_textures(void)
{
    this->board_texture = this->load_texture(BOARD_PATH);
    this->p1_texture = this->load_texture(P1_PATH);
    this->p2_texture = this->load_texture(P2_PATH);
}

void		GUI::set_buttons(void)
{
	this->buttons.push_back(Button(this->renderer, this->screen_height, this->offset, "RESET", this->btn_size, SCPRO_FONT, BG_COLOUR));
}

void		GUI::render_buttons(void)
{
	for (auto &btn : this->buttons)
		btn.render();
}

void		GUI::free_buttons(void)
{
	for (auto &btn : this->buttons)
		this->free_button(btn);
}

void		GUI::free_button(Button &btn)
{
	if (btn.font)
		TTF_CloseFont(btn.font);
	if (btn.texture)
		SDL_DestroyTexture(btn.texture);
}
