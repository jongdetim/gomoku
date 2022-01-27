#include "Board.hpp"
#include "GUI.hpp"
#include "PrimitiveGui.hpp"
#include <fstream>

#define PRINT(x) std::cout << x << std::endl

class RandomAi : public IAi
{
public:
	int	calculate(Board &board)
    {
        return random(0, (BOARDSIZE - 1));
    }
};

std::string get_random_name(void)
{
    std::string value;
    std::ifstream nameFileout;
    std::vector<std::string> tokens;
    std::string name("");
    int index;

    nameFileout.open("names.txt");
    while (std::getline(nameFileout, value))
    {
        tokens = tokenize(value, ',');
        index = random(0, tokens.size() - 1);
        name += tokens[index] + " ";
    }

    nameFileout.close();
    return name;
}

int main()
{
    // PrimitiveGui gui(NULL);
    // RandomAi ai;
    // GUI gui(&ai);


    // GUI gui(NULL);
    // Board board;
    
    // board.play(gui);

    std::cout << get_random_name() << std::endl;
    // get_random_name();

    return 0;
}
