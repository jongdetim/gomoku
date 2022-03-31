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
#ifndef IGAMEENGINE_HPP
# define IGAMEENGINE_HPP

class NegamaxAi;

enum e_action 
{
	def = 0,
	restart = 1,
	quit = (1<<1),
	pauze = (1<<2),
	undo = (1<<3)
};

class Board;

class IGameEngine
{
public:
	IGameEngine(NegamaxAi *ai) : ai(ai) {}

	virtual void	play(Board board) = 0;
	virtual void	reset(void) = 0;
	virtual void	gameloop(void) = 0;

	NegamaxAi		*ai;
};

#endif
