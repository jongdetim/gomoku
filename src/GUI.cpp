#include "GUI.hpp"

GUI::GUI(void) { }

bool		GUI::initiate_GUI(std::string title) { return this->initiate_GUI(title, SCREEN_WIDTH, SCREEN_HEIGHT); }

bool		GUI::initiate_GUI(std::string title, int width, int height)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
	}

	SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_RESIZABLE, &this->window, &this->renderer);

	if (window == NULL) {
        SDL_Log("Unable to create window: %s", SDL_GetError());
        return false;
    }

	return true;
}

GUI::~GUI()
{
    if (this->renderer)
		SDL_DestroyRenderer(this->renderer);
	if (this->window)
    	SDL_DestroyWindow(this->window);
	SDL_Quit();
}

void		GUI::game(const Board &board)
{
	SDL_Event e;

    while (handle_events(e) != quit)
	{
		this->draw_state(board.get_state());
    }
}

/* Private Methods */

void		GUI::draw_state(BITBOARD state)
{
}

game_loop	GUI::handle_events(SDL_Event &event)
{
	while (SDL_PollEvent(&event) )
	{                         
		if (event.type == SDL_QUIT)
			return quit;
		if(event.type == SDL_MOUSEBUTTONUP)
    	{
			//Get mouse position
			int x, y;
			SDL_GetMouseState( &x, &y );
			printf("Mouse clicked: X:%d, Y:%d\n", x, y);

			// SDL_RenderClear(this->renderer);

			SDL_Rect rect;
			rect.w = 200;
			rect.h = 200;
			rect.x = x - (rect.w >> 1);
			rect.y = y - (rect.h >> 1);

			SDL_SetRenderDrawColor(this->renderer, 255, 255, 255, 255);
			// SDL_RenderDrawRect(this->renderer, &rect);
			
			draw_circle(x, y, 100);

			// SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);

			SDL_RenderPresent(this->renderer);

			
		}
	}
	return loop;
}

void		GUI::update_window(void) { SDL_UpdateWindowSurface(this->window); }



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
