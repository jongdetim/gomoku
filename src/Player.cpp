#include "Player.hpp"
#include "misc.hpp"

/* ======PlayerFn======= */
std::vector<std::string>	tokenize(std::string &str, char delim)
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

bool						try_parse_input(std::string &input, int &out)
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

int							get_player_input(void)
{
	std::string input;
	int index;

	std::getline(std::cin, input);
	if (input == "quit" || input == "exit")
		return quit;
	if (!try_parse_input(input, index))
		return error;
	return index;
}
/* ===================== */

void						Player::set_fn(player_fn fn) { this->fn = !fn ? &get_player_input : fn; }

void						Player::print(void)
{
	std::cout << "Name." << this->name << std::endl;
	std::cout << "Id." << this->id << std::endl;
	std::cout << "IndexOffset." << this->index_offset << std::endl;
	std::cout << "Captures." << this->captures << std::endl;
	std::cout << "LastMove." << this->last_move << std::endl;
	std::cout << "StonesInPlay." << this->stones_in_play << std::endl;
}

void						Player::reset(void)
{
	this->last_move = -1;
	this->captures = 0;
	this->stones_in_play = 0;
	this->winning_index = -1;
}

bool						Player::has_wincondition(void) { return this->winning_index != -1; }

bool						Player::operator==(Player &rhs) const { return this->id == rhs.id; }

bool						Player::operator==(int id) const { return this->id == id; }

bool						Player::operator!=(Player &rhs) const { return  this->id != rhs.id; }

bool						Player::operator!=(int id) const { return  this->id != id; }
