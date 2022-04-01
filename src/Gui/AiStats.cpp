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
#include "AiStats.hpp"

AiStats::AiStats(void) : renderer(NULL) {}

AiStats::AiStats(SDL_Renderer *renderer, SDL_Rect rect, TTF_Font *font) :
stats{0,0,0,0,0}, rect(rect), renderer(renderer), font(font), pos{rect.x, rect.y}
{
	TTF_SizeText(this->font, "", NULL, &this->textH);
}

void		AiStats::init(int offset)
{
	int x = this->pos.x + offset;
	int y = this->pos.y + offset;

	this->duration = Text(this->renderer, t_point {x, y});
	this->depth = Text(this->renderer, t_point {x, y + (1 * this->textH)});
	this->nodes = Text(this->renderer, t_point {x, y + (2 * this->textH)});
	this->score = Text(this->renderer, t_point {x, y + (3 * this->textH)});
	this->nodes_ps = Text(this->renderer, t_point {x, y + (4 * this->textH)});
}

void		AiStats::render(t_aistats stats)
{
	SDL_RenderDrawRect(this->renderer, &this->rect);
	if (stats.depth)
	{
		this->update(stats);
		this->duration.render();
		this->depth.render();
		this->nodes.render();
		this->score.render();
		this->nodes_ps.render();
	}
}

void		AiStats::update(t_aistats stats)
{
	this->duration.update("Duration: " + std::to_string(stats.duration) + "ms", this->font);
	this->depth.update("Depth:      " + std::to_string(stats.depth), this->font);
	this->nodes.update("Visited:     " + std::to_string(stats.nodes_visited), this->font);
	this->score.update("Score:       " + std::to_string(stats.score), this->font);
	if (stats.duration > 0)
		this->nodes_ps.update("Nodes/s:  " + std::to_string((int)(stats.nodes_visited * (1000.0 / (float)stats.duration))), this->font);
	else
		this->nodes_ps.update("Nodes/s:  ", this->font);
}

void	AiStats::reset_stats(void) { this->stats = {0,0,0,0,0}; }
