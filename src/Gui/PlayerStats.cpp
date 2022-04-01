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

void		PlayerStats::update_font(int index, TTF_Font *font)
{
	this->texts[index].update(this->texts[index].text, font);
}

void		PlayerStats::update(GuiPlayer &player)
{
	this->texts[name_text].update(player.name, this->name_font);
	this->texts[empty_text].update(player.hint_active ? "*" : "-", this->font);
	this->texts[player_text].update((player.ai ? "computer" : "player "), this->font);
	this->texts[captures_text].update("captures  " + std::to_string(player.captures()), this->font);
}

void		PlayerStats::render(void)
{
	for (auto &txt : this->texts)
		txt.render();
}


t_point		PlayerStats::get_position(int index) const { return this->texts[index].get_pos(); }

t_point		PlayerStats::get_size(e_playerstats index) const  { return this->texts[index].get_size(); }

bool		PlayerStats::on_text(int x, int y, e_playerstats index) { return this->texts[index].on_text(x, y); }

bool		PlayerStats::is_active(e_playerstats index) const { return this->texts[index].is_active(); }

void		PlayerStats::free(void)
{
	for (int i = 0; i < size_playertexts; i++)
		this->texts[i].free();
}
