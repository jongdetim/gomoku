#include "Board.hpp"
#include "algorithm.hpp"
#include "misc.hpp"

#define PRINT(x) std::cout << x << std::endl
#define PRINTENDL std::cout << std::endl

int     main()
{
    Board board;
    int wins, amount = 0, player = PLAYER1;
    double average, perc;

    for (int i = 0; i < 20; i++)
    {
        amount += 100;
        average = 0;
        for (int seed = 0; seed < 10; seed++)
        {
            wins = play_random_games(board, player, amount, false, seed);
            perc = (wins / (double)amount) * 100;
            std::cout << "Games : " << amount << ", wins: " << perc << "%" << std::endl;
            average += perc;
        }
        std::cout << "Average : \t\t" << average / 10.0 << "%" << std::endl;
        PRINTENDL;
    }
    return 0;
}
