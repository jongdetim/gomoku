#include "Stats.hpp"

Stats::Stats(void) : renderer(NULL) {}

Stats::Stats(SDL_Renderer *renderer, Player *player, t_point pos, TTF_Font *font) :
player(player), renderer(renderer), pos(pos), font(font)
{
	TTF_SizeText(this->font, "", NULL, &this->textH);
}

void		Stats::init(void)
{
	for (int i = 0; i < 3; i++)
		this->texts.push_back(Text(this->renderer, t_point {this->pos.x, this->pos.y + (i * this->textH)}, this->font));
}

void		Stats::update(void)
{
	this->texts[0].update(this->player->name);
	this->texts[1].update("captures: " + std::to_string(this->player->captures));
	this->texts[2].update("stones  : " + std::to_string(this->player->stones_in_play));
}

void		Stats::render(void)
{
	for (auto &txt : this->texts)
		txt.render();
}
