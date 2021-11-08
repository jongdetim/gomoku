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

	this->window = SDL_CreateWindow(
        title.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_SHOWN
    );

	if (window == NULL) {
        SDL_Log("Unable to create window: %s", SDL_GetError());
        return false;
    }

	this->surface = SDL_GetWindowSurface(this->window);

	return true;
}

void		GUI::game(const Board &board)
{
	SDL_Event e;

    while (handle_events(e) != quit)
	{
		this->draw(board.get_state());
    }
}

GUI::~GUI()
{
    SDL_DestroyWindow(this->window);
    SDL_Quit();
}

/* Private Methods */

void		GUI::draw(BITBOARD state)
{
}

game_loop	GUI::handle_events(SDL_Event &e)
{
	while (SDL_PollEvent(&e) )
	{                         
		if (e.type == SDL_QUIT)
			return quit;
	}
	return loop;
}

void		GUI::update_window(void) { SDL_UpdateWindowSurface(this->window); }