#include "GUI.hpp"

GUI::GUI(void) { }

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

    this->image = SDL_LoadBMP(IMAGE_PATH);

	if (this->image == NULL) {
		SDL_Log("Unable to load background image: %s", SDL_GetError());
		return false;
    }

    this->texture = SDL_CreateTextureFromSurface(this->renderer, this->image);

	return true;
}

GUI::~GUI()
{
	if (this->image)
	    SDL_FreeSurface(this->image);
	if (this->texture)
    	SDL_DestroyTexture(this->texture);
    if (this->renderer)
		SDL_DestroyRenderer(this->renderer);
	if (this->window)
    	SDL_DestroyWindow(this->window);
	SDL_Quit();
}

void		GUI::game(const Board &board)
{
	bool quit = false;

	this->update_window();
    
    while (!quit)
    {
		// SDL_PollEvent(&this->event) // Use when always want updating, like active animations when no user input
        SDL_WaitEvent(&this->event);

		if (this->update)
			this->update_window();

		quit = this->handle_events();
    }
}

/* Private Methods */

void		GUI::draw_state(BITBOARD state)
{
}

void		GUI::clear_render(void)
{
	SDL_SetRenderDrawColor(renderer, 220, 179, 92, 255);
	SDL_RenderClear(renderer);
}

void		GUI::set_background(void)
{
	SDL_Rect bg_rect = { 0, 0, SCREEN_HEIGHT, SCREEN_HEIGHT };
	SDL_RenderCopy(this->renderer, this->texture, NULL, &bg_rect);
}

bool		GUI::handle_events(void)
{         
	switch (this->event.type)
	{
		case SDL_QUIT:
			return true;
		case SDL_MOUSEBUTTONUP:
		{
			int row, col;
			this->get_placement(&row, &col);
			// printf("Row:%d, Col:%d\n\n", row, col);
			static Board board;
			static int player = 1;

			board.place(row, col, player);
			player = -player;
			system("clear");
			board.show_last_move();

			this->update = true;

			break;
		}
	}
	return false;
}

void		GUI::update_window(void)
{
	this->clear_render();
	this->set_background();
	SDL_RenderPresent(this->renderer);
	this->update = false;
}

void		GUI::get_placement(int *row, int *col)
{
	SDL_GetMouseState(col, row);
	*row = (*row - OFFSET) / (double)GAP + .5;
	*col = (*col - OFFSET) / (double)GAP + .5;
}

void		GUI::draw_circle(int32_t centreX, int32_t centreY, int32_t radius)
{
   const int32_t diameter = (radius * 2);

   int32_t x = (radius - 1);
   int32_t y = 0;
   int32_t tx = 1;
   int32_t ty = 1;
   int32_t error = (tx - diameter);

   while (x >= y)
	{
		//  Each of the following renders an octant of the circle
		SDL_RenderDrawPoint(this->renderer, centreX + x, centreY - y);
		SDL_RenderDrawPoint(this->renderer, centreX + x, centreY + y);
		SDL_RenderDrawPoint(this->renderer, centreX - x, centreY - y);
		SDL_RenderDrawPoint(this->renderer, centreX - x, centreY + y);
		SDL_RenderDrawPoint(this->renderer, centreX + y, centreY - x);
		SDL_RenderDrawPoint(this->renderer, centreX + y, centreY + x);
		SDL_RenderDrawPoint(this->renderer, centreX - y, centreY - x);
		SDL_RenderDrawPoint(this->renderer, centreX - y, centreY + x);

		if (error <= 0)
		{
			++y;
			error += ty;
			ty += 2;
		}

		if (error > 0)
		{
			--x;
			tx += 2;
			error += (tx - diameter);
		}
   }
}
