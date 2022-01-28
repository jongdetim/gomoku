#include "Stats.hpp"

Stats::Stats(void) : renderer(NULL) {}

Stats::Stats(SDL_Renderer *renderer, Player *player, t_point pos, TTF_Font *font) :
player(player), renderer(renderer), pos(pos), font(font)
{
	TTF_SizeText(this->font, "", NULL, &this->textH);
}

void		Stats::init(void)
{
	for (int i = 0; i < size_texts; i++)
		this->texts.push_back(Text(this->renderer, t_point {this->pos.x, this->pos.y + (i * this->textH)}, this->font));
}

void		Stats::update(void)
{
	this->texts[name_text].update(this->player->name);
	this->texts[captures_text].update("captures " + std::to_string(this->player->captures));
	this->texts[stones_text].update	 ("stones   " + std::to_string(this->player->stones_in_play));
	this->texts[ai_text].update		 ("ai       " + (std::string)(this->player->ai ? "on" : "off"));
}

void		Stats::render(void)
{
	for (auto &txt : this->texts)
		txt.render();
}
