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
#ifndef AISTATS_HPP
# define AISTATS_HPP

# include <SDL.h>
# include <SDL_ttf.h>
# include "Text.hpp"
# include "algorithm.hpp"

class AiStats
{
public:
	AiStats(void);
	AiStats(SDL_Renderer *renderer, SDL_Rect rect, TTF_Font *font);

	t_aistats			stats;

	void				render(t_aistats stats);
	void				init(int offset);
	
private:
	void				update(t_aistats stats);

	SDL_Rect			rect;
	Text				duration;
	Text				depth;
	Text				nodes;
	Text				score;
	Text				nodes_ps;
	SDL_Renderer		*renderer;
	TTF_Font			*font;
	int 				textH;
	t_point				pos;
};

#endif