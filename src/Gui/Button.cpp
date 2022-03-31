/* MIT License

Copyright (c) 2022 Flint Louis

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 */
#include "Button.hpp"

Button::Button(SDL_Renderer *renderer, int x, int y, std::string text, TTF_Font *font, SDL_Color colour, short action) :
active(false), texture(NULL), pos(t_point {x, y}), text(text), colour(colour), renderer(renderer), action(action), font(font) { }

Button::~Button()
{
	if (this->texture)
		SDL_DestroyTexture(this->texture);
}

void		Button::init(void)
{
	int texW, texH;

	SDL_Surface *surface = TTF_RenderText_Solid(this->font, this->text.c_str(), SDL_Color{0,0,0,255});
	this->texture = SDL_CreateTextureFromSurface(this->renderer, surface);
	SDL_FreeSurface(surface);

	SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
	this->rect = SDL_Rect { this->pos.x, this->pos.y, texW, texH };
}

void		Button::render(void)
{
	SDL_Color c = this->active ? this->subtract_colour(this->colour, SUBTRACT) : this->colour;

	SDL_SetRenderDrawColor(this->renderer, c.r, c.g, c.b, 255);
	SDL_RenderFillRect(this->renderer, &rect);
	SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);
	SDL_RenderDrawRect(this->renderer, &rect);

	SDL_RenderCopy(this->renderer, texture, NULL, &rect);
}

SDL_Colour	Button::subtract_colour(SDL_Colour col, int amount) const
{
	SDL_Colour colour;

	colour.r = (col.r - amount) < 0 ? 0 : (col.r - amount);
	colour.g = (col.g - amount) < 0 ? 0 : (col.g - amount);
	colour.b = (col.b - amount) < 0 ? 0 : (col.b - amount);
	colour.a = 255;

	return colour;
}

bool		Button::is_active(void) { return this->active; }

bool		Button::on_button(int x, int y)
{
	bool prev = this->active;
	this->active = (x > this->rect.x && x <= (this->rect.x + this->rect.w)) && (y > this->rect.y && y <= (this->rect.y + this->rect.h));
	return prev != this->active;
}

short		Button::get_action(void) { return this->action; }

t_point		Button::get_button_size(void)
{
	t_point size;

	TTF_SizeText(this->font, this->text.c_str(), &size.x, &size.y);
	return size;
}
