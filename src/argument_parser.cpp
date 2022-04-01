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
#include "argument_parser.hpp"

void                    argument_parser::set_options(argparse::ArgumentParser &program)
{
    program.add_argument("--size", "-s")
    .default_value("big")
    .required()
    .help("GUI size (big|medium|small)");

    program.add_argument("--replay", "-r")
    .help("Replay game from log (starting board: log/(id).board.data)");
}

void					argument_parser::parse_args(argparse::ArgumentParser &program, int argc, char **argv)
{
    try
    {
        program.parse_args(argc, argv);
        
        if (program.is_used("--size"))
        {
            auto size = program.get<std::string>("--size");
            if (size != "small" || size != "medium" || size != "big")
                throw "--size: no acceptable value provided.";
        }
    }
    catch (const std::runtime_error& err)
    {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        std::exit(1);
    }
    catch (char const* &str)
    {
        std::cerr << str << '\n';
        std::cerr << program;
        exit(1);
    }
}

GUI                     argument_parser::get_gui(argparse::ArgumentParser &program, NegamaxAi &ai)
{
    std::string size;
    
    if (program.is_used("--size"))
    {
        auto size = program.get<std::string>("--size");
        return (size == "small") ? GUI(&ai, small) : (size == "medium") ? GUI(&ai, medium) : GUI(&ai, big);
    }
    return GUI(&ai);
}
