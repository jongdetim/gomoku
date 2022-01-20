#include "Text.hpp"

Text::Text(void) : renderer(NULL), font(NULL), texture(NULL) { }

Text::Text(SDL_Renderer *renderer, t_point pos, TTF_Font *font) : renderer(renderer), font(font), texture(NULL), pos(pos) { }

Text::~Text()
{
	if (this->texture)
		SDL_DestroyTexture(this->texture);
}

void		Text::update(std::string text)
{
	if (this->texture)
		SDL_DestroyTexture(this->texture);

	SDL_Surface *surface = TTF_RenderText_Solid(this->font, text.c_str(), SDL_Color{0,0,0,255});
	this->texture = SDL_CreateTextureFromSurface(this->renderer, surface);
	SDL_FreeSurface(surface);
}

void		Text::render(void)
{
	int texW, texH;

	SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
	auto rect = SDL_Rect { this->pos.x, this->pos.y, texW, texH };

	SDL_RenderCopy(this->renderer, this->texture, NULL, &rect);
}