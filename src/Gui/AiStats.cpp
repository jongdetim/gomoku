#include "AiStats.hpp"

AiStats::AiStats(void) : renderer(NULL) {}

AiStats::AiStats(SDL_Renderer *renderer, SDL_Rect rect, TTF_Font *font) :
renderer(renderer), pos{rect.x, rect.y}, font(font), rect(rect), stats{0}
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
	this->nodes_ps.update("Nodes/s:  " + std::to_string((int)(stats.nodes_visited * (1000.0 / (float)stats.duration))), this->font);
}
