#include "PlayerStats.hpp"

PlayerStats::PlayerStats(void) : renderer(NULL) {}

PlayerStats::PlayerStats(SDL_Renderer *renderer, t_point pos, TTF_Font *font, TTF_Font *name_font) :
renderer(renderer), pos(pos), font(font), name_font(name_font)
{
	TTF_SizeText(this->font, "", NULL, &this->textH);
}

void		PlayerStats::init(void)
{
	for (int i = 0; i < size_playertexts; i++)
		this->texts[i] = Text(this->renderer, t_point {this->pos.x, this->pos.y + (i * this->textH)});
}

void		PlayerStats::update(GuiPlayer &player)
{
	this->texts[name_text].update(player.name, this->name_font);
	this->texts[empty_text].update("-", this->font);
	this->texts[player_text].update((player.ai ? "computer" : "player "), this->font);
	this->texts[captures_text].update("captures  " + std::to_string(player.captures()), this->font);
	this->texts[stones_text].update("stones  " + std::to_string(player.stones()), this->font);
}

void		PlayerStats::render(void)
{
	for (auto &txt : this->texts)
		txt.render();
}


t_point		PlayerStats::get_position(int index) const { return this->texts[index].get_pos(); }

t_point		PlayerStats::get_size(e_playerstats index) const  { return this->texts[index].get_size(); }

bool		PlayerStats::on_text(int x, int y, e_playerstats index) const { return this->texts[index].on_text(x, y); }
