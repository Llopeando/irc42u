#include <iostream>
#include "include/utils.hpp"

void printTime()
{

}

int main()
{
	utils::t_chrono::time_point now = utils::t_chrono::now();

	std::cout << now << "\n";
	return 0;
}