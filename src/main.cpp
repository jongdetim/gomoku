/* MIT License

Copyright (c) 2022 Flint Louis, Tim de Jong

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
#include "Board.hpp"
#include "argument_parser.hpp"

int                     main(int argc, char **argv)
{
    NegamaxAi nai;
    argparse::ArgumentParser program("Gomoku");

    argument_parser::set_options(program);
    argument_parser::parse_args(program, argc, argv);

    Board::init_zobrist_map();

    GUI gui = argument_parser::get_gui(program, nai);    
    if (program.is_used("--replay"))
    {
        try
        {
            gui.replay(program.get<std::string>("--replay"));
        }
        catch(const char *e)
        {
            std::cerr << e << std::endl;;
            std::cerr << program;
            std::exit(1);
        }
    }
    else
        gui.play(Board());

    return 0;
}
