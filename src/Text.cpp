#include "Text.hpp"

Text::Text(void) : renderer(NULL), texture(NULL) { }

Text::Text(SDL_Renderer *renderer, t_point pos) : renderer(renderer), texture(NULL), pos(pos) { }

Text::~Text()
{
	if (this->texture)
		SDL_DestroyTexture(this->texture);
}

void		Text::update(std::string text, TTF_Font *font)
{
	if (this->texture)
		SDL_DestroyTexture(this->texture);

	SDL_Surface *surface = TTF_RenderText_Solid(font, text.c_str(), SDL_Color{0,0,0,255});
	this->texture = SDL_CreateTextureFromSurface(this->renderer, surface);
	SDL_FreeSurface(surface);
}

void		Text::render(void)
{
	int texW, texH;

	SDL_QueryTexture(this->texture, NULL, NULL, &texW, &texH);
	SDL_Rect rect = { this->pos.x, this->pos.y, texW, texH };

	SDL_RenderCopy(this->renderer, this->texture, NULL, &rect);
}
