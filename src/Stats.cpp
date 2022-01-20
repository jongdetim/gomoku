#include "Stats.hpp"

Stats::Stats(void) : renderer(NULL) {}

Stats::Stats(SDL_Renderer *renderer, Player *player, int x, int y, TTF_Font *font) :
player(player), renderer(renderer), pos(Eigen::Array2i {x, y}), font(font) { }

Stats::Text::Text(int x, int y, TTF_Font *font) : font(font), texture(NULL), pos(Eigen::Array2i {x, y}) { }

Stats::Text::~Text()
{
	if (this->texture)
		SDL_DestroyTexture(this->texture);
}

void		Stats::Text::update(std::string text, SDL_Renderer *renderer)
{
	// int texW, texH;
	if (this->texture)
		SDL_DestroyTexture(this->texture);

	SDL_Surface *surface = TTF_RenderText_Solid(this->font, text.c_str(), SDL_Color{0,0,0,255});
	this->texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	// SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
	// this->rect = SDL_Rect { this->pos[0], this->pos[1], texW, texH };
}

void		Stats::Text::render(SDL_Renderer *renderer)
{
	int texW, texH;

	SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
	auto rect = SDL_Rect { this->pos[0], this->pos[1], texW, texH };

	SDL_RenderCopy(renderer, this->texture, NULL, &rect);
}

void		Stats::init(void)
{

	this->texts.push_back(Text(this->pos[0], this->pos[1], this->font));

	// printf("Name.%-*s Name.%s\n", 18, PLAYER1.name.c_str(), PLAYER2.name.c_str());
	// printf("Captures.%-*d Captures.%d\n", 14, PLAYER1.captures, PLAYER2.captures);
	// printf("LastMove.%-*d LastMove.%d\n", 14, PLAYER1.last_move, PLAYER2.last_move);
	// printf("StonesInPlay.%-*d StonesInPlay.%d\n", 10, PLAYER1.stones_in_play, PLAYER2.stones_in_play);
	// printf("WinningIndex.%-*d WinningIndex.%d\n", 10, PLAYER1.winning_index, PLAYER2.winning_index);
}

void		Stats::update(void)
{
	std::stringstream strm;
	strm << "captures: " << this->player->captures;

	this->texts[0].update(strm.str(), this->renderer);
}

void		Stats::render(void)
{
	for (auto &txt : this->texts)
		txt.render(this->renderer);
}
