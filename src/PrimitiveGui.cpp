#include "PrimitiveGui.hpp"

void						PrimitiveGui::play(Board *board)
{
	this->reset(*board);
	this->gameloop(*board);
}

void						PrimitiveGui::gameloop(Board &board)
{
	int		index;

	while (true)
	{
		this->print_stats(board);
		board.show_move();

		index = this->get_index(board);

		if (index == -1 || !board.place(index))
			continue;
		
		if (board.is_game_finished(*board.current_player))
			break;
		
		board.next_player();
	}
	board.switch_to_player(board.winner);
	this->print_winner(board);
}

void						PrimitiveGui::reset(Board &board)
{
	board.reset();
	board.random_player();
}

void						PrimitiveGui::print_stats(Board &board) const
{
	system("clear");
	std::cout << std::endl;

	printf("Name.%-*s Name.%s\n", 18, board.player1.name.c_str(), board.player2.name.c_str());
	printf("Captures.%-*d Captures.%d\n", 14, board.player1.captures, board.player2.captures);
	printf("StonesInPlay.%-*d StonesInPlay.%d\n", 10, board.player1.stones_in_play, board.player2.stones_in_play);
	std::cout << std::endl;
	std::cout << "Current Player: " << board.current_player->name;
	char symbol = (board.current_player->id == PLAYER1_ID ? P1_SYMBOL-32 : P2_SYMBOL-32);
	std::cout << ' ' << symbol << std::endl;
	std::cout << std::endl;
}

std::vector<std::string>	PrimitiveGui::tokenize(std::string &str, char delim)
{
	size_t start;
	size_t end = 0;
    std::vector<std::string> tokens;

	while ((start = str.find_first_not_of(delim, end)) != std::string::npos)
	{
		end = str.find(delim, start);
		tokens.push_back(str.substr(start, end - start));
	}
	return tokens;
}

bool						PrimitiveGui::try_parse_input(std::string &input, int &out)
{
	auto tokens = tokenize(input, ' ');
	int col;

	if (tokens.size() > 2 || tokens.size() == 0)
		return false;

	try
	{
		out = std::stoi(tokens[0]);
		if (tokens.size() == 2)
		{
			col = std::stoi(tokens[1]);
			out = calc_index(out, col);
		}
	} catch (...) { return false; }

	return true;
}

int							PrimitiveGui::get_player_input(void)
{
	std::string input;
	int index;

	std::getline(std::cin, input);
	if (!try_parse_input(input, index))
		return -1;
	return index;
}

int							PrimitiveGui::get_index(Board &board)
{
	if (board.current_player->id == PLAYER1_ID)
		return this->get_player_input();
	/* else if (this->ai)
		return this->ai.(); */
	else
		return this->get_player_input();
}

void						PrimitiveGui::print_winner(Board &board) const
{
	this->print_stats(board);
	board.show_move(board.current_player->last_move);
	if (!board.is_full())
		std::cout << "*** " << board.current_player->name << " WINS!!! ***" << std::endl;
}
