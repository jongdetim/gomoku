/* MIT License

Copyright (c) 2022 Flint Louis, Tim de Jong

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
#include "Text.hpp"

Text::Text(void) : renderer(NULL), texture(NULL) { }

Text::Text(SDL_Renderer *renderer, t_point pos) :
	text(""),
	renderer(renderer),
	texture(NULL),
	pos(pos),
	active(false)
{ }

void		Text::update(std::string text, TTF_Font *font)
{
	if (this->texture)
		SDL_DestroyTexture(this->texture);

	this->text = text;
	SDL_Surface *surface = TTF_RenderText_Blended(font, text.c_str(), SDL_Color{0,0,0,255});
	this->texture = SDL_CreateTextureFromSurface(this->renderer, surface);
	SDL_FreeSurface(surface);
}

void		Text::render(void)
{
	SDL_QueryTexture(this->texture, NULL, NULL, &this->size.x, &this->size.y);
	SDL_Rect rect = { this->pos.x, this->pos.y, this->size.x, this->size.y };

	SDL_RenderCopy(this->renderer, this->texture, NULL, &rect);
}

t_point		Text::get_pos(void) const { return this->pos; }

t_point		Text::get_size(void) const  { return this->size; }

bool		Text::on_text(int x, int y)
{
	bool prev = this->active;
	this->active = (x > this->pos.x && x <= (this->pos.x + this->size.x)) && (y > this->pos.y && y <= (this->pos.y + this->size.y));
	return prev != this->active;
}

bool		Text::is_active(void) const { return this->active; }

void		Text::free(void)
{
	if (this->texture)
		SDL_DestroyTexture(this->texture);
}
