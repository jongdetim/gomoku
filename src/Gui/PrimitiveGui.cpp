#include "PrimitiveGui.hpp"
#include "Board.hpp"
#include "misc.hpp"

PrimitiveGui::PrimitiveGui(IAi *ai) : IGameEngine(ai) {}

void						PrimitiveGui::play(Board *board)
{
	this->guiboard = GuiBoard(*board);
	this->guiboard.players[PLAYER1].name = "P1";
	this->guiboard.players[PLAYER2].name = "P2";
	this->guiboard.players[PLAYER2].ai = this->ai;

	gameloop();
}

void						PrimitiveGui::gameloop(void)
{
	int		index;

	while (this->action != quit)
	{
		print_stats();
		GUIBOARD.show_last_move();

		index = get_index();

		if (GUIBOARD.place(index))
		{
			if (GUIBOARD.is_game_finished())
				break;
			
			GUIBOARD.next_player();
		}

		if (this->action == restart)
			reset();
	}
	if (GUIBOARD.has_winner())
		print_winner();
}

void						PrimitiveGui::reset(void)
{
	GUIBOARD.reset();
	GUIBOARD.random_player();
	this->action = def;
}

void						PrimitiveGui::print_stats(void)
{
	system("clear");
	std::cout << std::endl;
	printf("Name.%-*s Name.%s\n", 18, this->guiboard.players[PLAYER1].name.c_str(), this->guiboard.players[PLAYER2].name.c_str());
	printf("Captures.%-*d Captures.%d\n", 14, this->guiboard.players[PLAYER1].captures(), this->guiboard.players[PLAYER2].captures());
	std::cout << std::endl;
	std::cout << "Current Player: " << this->guiboard.current_player().name;
	char symbol = (GUIBOARD.get_current_player() == PLAYER1 ? P1_SYMBOL-32 : P2_SYMBOL-32);
	std::cout << ' ' << symbol << std::endl;
	std::cout << std::endl;
}

bool						PrimitiveGui::try_parse_input(std::string &input, int &out)
{
	auto tokens = misc::tokenize(input, ' ');
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

int							PrimitiveGui::get_index(void)
{
	if (this->guiboard.current_player().ai)
		return this->guiboard.current_player().ai->calculate(this->guiboard.get_board());
	else
		return get_player_input();
}

void						PrimitiveGui::print_winner(void)
{
	int winner = GUIBOARD.winner;

	print_stats();
	GUIBOARD.show_move(this->guiboard.players[winner].last_move());
	std::cout << "*** " << this->guiboard.players[winner].name << " WINS!!! ***" << std::endl;
}
