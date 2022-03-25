#include "Text.hpp"

Text::Text(void) : renderer(NULL), texture(NULL) { }

Text::Text(SDL_Renderer *renderer, t_point pos) :
	text(""),
	renderer(renderer),
	texture(NULL),
	pos(pos),
	active(false)
{ }

Text::~Text()
{
	if (this->texture)
		SDL_DestroyTexture(this->texture);
}

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
