# include "gomoku.hpp"
# include "Heuristic.hpp"
# include "Board.hpp"
# include "TranspositionTable.hpp"
# include "misc.hpp"
# include "algorithm.hpp"

#define PRINT(x) std::cout << x << std::endl

void               cutout_pattern(const Board &board, int move, int direction, int player, t_pattern &pat)
{
    int shift = DIRECTIONS[direction + 4];
    int pos = move - (pat.left_right[0] * shift);
    for (int i = 0; i < pat.length; i++)
    {
        pat.pattern += abs(board.get_player(pos));
        if (i != pat.length - 1)
            pat.pattern <<= 1;
        pos += shift;
    }
    // PRINT(std::bitset<8>(pat.pattern));
}

t_pattern			get_pattern_data(Board &board, int move, int direction, int player, std::bitset<BOARDSIZE> *checked_indices)
{
	t_pattern pattern;
	int pos;
	pattern.space = 1;
	pattern.count = 1;
	int shift;
    checked_indices[direction][move] = 1;
	for (int j = 0; j < 2; j++)
	{
		pos = move;
		shift = DIRECTIONS[direction + 4 * j];
		int i = 0;
		int open = 0;
		while (open < 3)
		{
			pos += shift;
            if (open == 0)
			{
                if (is_offside(pos - shift, pos) || board.get_player(pos) == -player)
                {
                    pattern.left_right[j] = i;
                    break;
                }
                if (board.get_player(pos) == player)
                {
                    pattern.count++;
                    checked_indices[direction][pos] = 1;
                }
                else if (board.get_player(pos - shift) == player) //leeg vakje maar vorige niet
                    pattern.left_right[j] = i + 1;
                else
                    open = 2;
            }
			else if (is_offside(pos - shift, pos) || board.get_player(pos) == -player)
                break;
            else
				open++; 
			pattern.space++;
			i++;
		}
	}
	pattern.length = pattern.left_right[0] + pattern.left_right[1] + 1;
	return pattern;
}

// bool search_subpattern(t_pattern &pat, t_pattern &sub)
// {
//     int8_t times = pat.length - sub.length;

//     if (times < 0)
//     {
//         PRINT("subpattern does not fit inside of cutout pattern");
//         exit(1);
//     }
//     for (int8_t i = 0; i < times + 1; i++)
//     {
//         PRINT(std::bitset<8>(uint8_t(pat.pattern << (8 - pat.length + i)) >> (times + (8 - pat.length))));
//         if(uint8_t(pat.pattern << (8 - pat.length + i)) >> (times + (8 - pat.length)) == sub.pattern)
//         {
//             // std::cout << std::bitset<8>(uint8_t(pat.pattern << i) >> times) << std::endl;
//             PRINT("PATTERN FOUND");
//             return true;
//         }
//     }
//     return false;
// }

void    print_and_quit(const char *msg)
{
    PRINT(msg);
    exit(1);
}

// unfinished, needs to assign a score to combinations and patterns
void score_heuristic_data(Board &board)
{
    for (uint8_t j = 0; j < 2; j++)
    {
        PRINT("PLAYER: " << (j + 0));
        for (uint8_t i = 0; i < 8; i++) // skip first index which is none
        {
            if (i > 0 && board.players[j].heuristic.patterns[i] > 0)
            {
                PRINT(PatternNames[i]);
                PRINT((int)board.players[j].heuristic.patterns[i]);
            }
        }
    }
}

// unfinished, needs to assign a score to combinations and patterns
void score_heuristic_data_index(Board &board, int move)
{
    int player = board.get_player(move);
    int index = player < 0 ? 0 : 1;
    for (uint8_t i = 0; i < 8; i++) // skip first index which is none
    {
        if (i > 0 && board.players[index].heuristic.patterns[i] > 0)
        {
            PRINT(PatternNames[i]);
            PRINT((int)board.players[index].heuristic.patterns[i]);
        }
    }
}

Pattern find_subpattern(t_pattern &pat, uint8_t length, const std::map<uint8_t, Pattern> &map)
{
    int8_t fit = pat.length - length;
    Pattern result = none;

    if (fit < 0)
        print_and_quit("cutout pattern is too smol");
    for (int8_t i = 0; i < fit + 1; i++)
    {
        auto iter = map.find(uint8_t(pat.pattern << (8 - pat.length + i)) >> (fit + (8 - pat.length)));
        if (iter != map.end() && iter->second > result)
        {
            result = iter->second;
            if (result == five || result == open4)
                return result;
        }
    }
    return result;
}

Pattern get_heuristic_data(Board &board, const int &move, const int &direction, const int &player, std::bitset<BOARDSIZE> *checked_indices)
{
    // board.heuristic.score = 0;
    // board.heuristic.patterns = {0};

    Pattern result = none;

    t_pattern pat = get_pattern_data(board, move, direction, player, checked_indices);

    // PRINT((int)pat.count);
    // PRINT((int)pat.left_right[0]);
    // PRINT((int)pat.left_right[1]);
    // PRINT((int)pat.length);
    // PRINT((int)pat.space);
    // PRINT(std::bitset<8>(pat.pattern));

    if (pat.count <= 1 || pat.space < 5)
        return none;
    
    if (pat.count == 2)
    {
        cutout_pattern(board, move, direction, player, pat);
        if (pat.space > 5 && ((pat.pattern == 0b00000110 && pat.length == 4) || (pat.pattern == 0b00001010 && pat.length == 5))) // .xx. with > 5 space OR .x.x. with > 5 space
            return open2;
        else
            return closed2;
    }
    cutout_pattern(board, move, direction, player, pat);
    if (pat.space == 5 && pat.count == 3) // |x.x.x| & |.xxx.| & |.xx.x| etc.
            return closed3;
    if (pat.length >= 5)
        result = find_subpattern(pat, 5, SUBPATTERNS_5);
    if (result != five && pat.length >= 6)
        result = std::max(result, find_subpattern(pat, 6, SUBPATTERNS_6));
    if (result == none) // xxx. & .xxx & xx.x.x & x.x.x.x & x.x.x. & x.x.x
        return closed3;
    return result;
}

void    get_heuristic_single(Board &board, int move, std::bitset<BOARDSIZE> *checked_indices)
{
    int player = board.get_player(move);
    int index = player < 0 ? 0 : 1;
    // PRINT("move: " << move);

    for (int dir = 0; dir < 4; dir++) // four directions
    {
        if (checked_indices[dir][move] == 1)
            continue;
        Pattern pattern = get_heuristic_data(board, move, dir, player, checked_indices);
        board.players[index].heuristic.patterns[pattern] += 1;

        // if (pattern != none) // print
        //     PRINT(PatternNames[pattern]);
    }
    // score_heuristic_data_index(board, move);
}

void    get_heuristic_total(Board &board)
{
    std::bitset<BOARDSIZE> checked_indices[4] = {0, 0, 0, 0};

    // first check the patterns at last_move just to be sure it is correctly registered
    get_heuristic_single(board, board.get_last_move(), checked_indices);
    for (int pos = 0; pos < BOARDSIZE; pos++)
    {
        if (board.filled_pos[pos])
            get_heuristic_single(board, pos, checked_indices);
    }
    // score_heuristic_data(board);
}

void    test()
{
    Board board;
    int index = calc_index(8, 8);
    // int star_index = calc_index(3, 16); 

    // create_star(board, star_index, 3, PLAYER1);
    // create_star(board, star_index, 4, PLAYER2);
    
    // board.remove(star_index);

    // board.place(star_index, PLAYER2);
    // std::cout << "captures : " << board.get_player_captures(PLAYER2) << std::endl;

    board.place(index - 1, PLAYER1);
    board.place(index - 3, PLAYER1);
    board.place(index, PLAYER1);
    board.place(index + 1, PLAYER2);
    // board.place(index - 21, PLAYER2);
    board.place(index + 2, PLAYER1);
    // #include "heuristic.hpp" 
    // PRINT(board.calc_heuristic());
    TranspositionTable h_table;
    TranspositionTable t_table;
    for (int depth = 1; depth < 7; depth++)
        negamax(board, depth, -std::numeric_limits<int>::max(), std::numeric_limits<int>::max(), PLAYER2, t_table, h_table, true);
    PRINT(TOTAL_NODES);
    PRINT(TOTAL_LEAVES);
    // board.place(360, PLAYER1);
    // Pattern result;
    // board = create_random_board(1);
    // for (int i = 0; i < 230000; i++)
    // {
    //     // ooooo.o.o. -> 000.0.0. would be solved by using 16 bits. what would be the performance impact?
    //     // board = create_random_board(1);

    //     // get the pattern
    //     // Pattern result = get_heuristic_data(board, board.get_last_move(), 0, board.get_last_player());
    //     // board.show_last_move();
    //     // get_heuristic_single(board, board.get_last_move());
    //     get_heuristic_total(board);
    //     // if (board.heuristic.patterns[0] < 4)
    //     // {  
    //     //     // board.show_last_move();
    //     //     // PRINT(PatternNames[result]);
    //     // }
    // }

    // see if some sub-pattern is inside the cutout pattern
    // t_pattern sub;
    // sub.pattern = 0b00000001;
    // sub.length = 3;
    
    // if (board.is_game_won())
    //     std::cout << "Won!!!" << std::endl;
    // else
    //     std::cout << "Not Won :(" << std::endl;

}

int     main()
{
    test();
    return 0;
}
