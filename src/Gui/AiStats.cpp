#include "AiStats.hpp"

AiStats::AiStats(void) : renderer(NULL) {}

AiStats::AiStats(SDL_Renderer *renderer, SDL_Rect rect, TTF_Font *font) :
renderer(renderer), pos{rect.x, rect.y}, font(font), rect(rect)
{
	TTF_SizeText(this->font, "", NULL, &this->textH);
}

void		AiStats::init(void)
{
	for (int i = 0; i < size_aitexts; i++)
		this->texts.push_back(Text(this->renderer, t_point {this->pos.x, this->pos.y + (i * this->textH)}));
}

// void		PlayerStats::update(GuiPlayer &player)
// {
	// this->texts[name_text].update(player.name, this->name_font);
	// this->texts[empty_text].update("-", this->font);
	// this->texts[player_text].update((player.ai ? "computer " : "player       "), this->font);
	// this->texts[captures_text].update("captures   " + std::to_string(player.captures()), this->font);
// }

void		AiStats::render(void)
{
	SDL_RenderDrawRect(this->renderer, &this->rect);
}
