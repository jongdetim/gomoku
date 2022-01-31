#include "PrimitiveGui.hpp"

PrimitiveGui::PrimitiveGui(IAi *ai) : IGameEngine(ai) {}

void						PrimitiveGui::play(Board *board)
{
	reset(*board);
	board->players[PLAYER1].ai = this->ai;
	gameloop(*board);
}

void						PrimitiveGui::gameloop(Board &board)
{
	int		index;

	while (this->action != quit)
	{
		print_stats(board);
		board.show_move();

		index = get_index(board);

		if (board.place(index))
		{
			if (board.is_game_finished())
				break;
			
			board.next_player();
		}

		if (this->action == restart)
			reset(board);
	}
	if (board.has_winner())
		print_winner(board);
}

void						PrimitiveGui::reset(Board &board)
{
	board.reset();
	board.random_player();
	this->action = def;
}

void						PrimitiveGui::print_stats(Board &board) const
{
	system("clear");
	std::cout << std::endl;

	printf("Name.%-*s Name.%s\n", 18, board.players[PLAYER1].name.c_str(), board.players[PLAYER2].name.c_str());
	printf("Captures.%-*d Captures.%d\n", 14, board.players[PLAYER1].captures, board.players[PLAYER2].captures);
	printf("StonesInPlay.%-*d StonesInPlay.%d\n", 10, board.players[PLAYER1].stones_in_play, board.players[PLAYER2].stones_in_play);
	std::cout << std::endl;
	std::cout << "Current Player: " << board.current_player()->name;
	char symbol = (board.current_player()->id == PLAYER1_ID ? P1_SYMBOL-32 : P2_SYMBOL-32);
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
			out = misc::calc_index(out, col);
		}
	} catch (...) { return false; }

	return true;
}

int							PrimitiveGui::get_player_input(void)
{
	std::string input;
	int index = -1;

	std::getline(std::cin, input);
	if (input == "quit" || input == "exit")
		this->action = quit;
	else if (input == "restart" || input == "reset")
		this->action = restart;
	else if (!try_parse_input(input, index))
		return -1;
	return index;
}

int							PrimitiveGui::get_index(Board &board)
{
	if (board.current_player()->ai)
		return board.current_player()->ai->calculate(board);
	else
		return get_player_input();
}

void						PrimitiveGui::print_winner(Board &board) const
{
	print_stats(board);
	board.show_move(board.current_player()->last_move);
	if (!board.is_full())
		std::cout << "*** " << board.current_player()->name << " WINS!!! ***" << std::endl;
}
