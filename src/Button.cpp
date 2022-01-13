#include "Button.hpp"

Button::Button(void) : font(NULL), texture(NULL) { }

Button::Button(SDL_Renderer *renderer, Eigen::Array2i pos, std::string text, int size, std::string font, SDL_Color colour) : pos(pos), text(text), colour(colour), texture(NULL), active(false), renderer(renderer)
{
	this->font = TTF_OpenFont(font.c_str(), size);
	if (!this->font)
		throw "Font can't be loaded.";
	this->init_button();
}

Button::Button(SDL_Renderer *renderer, int x, int y, std::string text, int size, std::string font, SDL_Color colour) : pos(Eigen::Array2i {x, y}), text(text), colour(colour), texture(NULL), active(false), renderer(renderer)
{
	this->font = TTF_OpenFont(font.c_str(), size);
	if (!this->font)
		throw "Font can't be loaded.";
	this->init_button();
}

void		Button::init_button(void)
{
	int texW, texH;

	SDL_Surface *surface = TTF_RenderText_Solid(this->font, this->text.c_str(), SDL_Color{0,0,0,255});
	this->texture = SDL_CreateTextureFromSurface(this->renderer, surface);
	SDL_FreeSurface(surface);

	SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
	this->rect = SDL_Rect { this->pos[0], this->pos[1], texW, texH };
}

void		Button::render(void)
{
	SDL_Color c = this->active ? this->subtract_colour(this->colour, 20) : this->colour;

	SDL_SetRenderDrawColor(this->renderer, c.r, c.g, c.b, 255);
	SDL_RenderFillRect(this->renderer, &rect);
	SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);
	SDL_RenderDrawRect(this->renderer, &rect);

	SDL_RenderCopy(this->renderer, texture, NULL, &rect);
}

SDL_Colour	Button::subtract_colour(SDL_Colour col, int amount) const
{
	SDL_Colour colour;

	colour.r = col.r - amount;
	colour.g = col.g - amount;
	colour.b = col.b - amount;
	colour.a = 255;

	return colour;
}

bool		Button::is_active(int x, int y)
{
	bool prev = this->active;
	this->active = (x > this->rect.x && x <= (this->rect.x + this->rect.w)) && (y > this->rect.y && y <= (this->rect.y + this->rect.h));
	return prev != this->active;
}
