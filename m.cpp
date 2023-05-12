

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <cstdio>
#include <unistd.h>
#include <poll.h>
#include <deque>
#include <fcntl.h>


std::vector<std::string> spliti(const std::string &string, const char c)
{
	std::vector<std::string> finalString;
	bool		lock = false;
	uint32_t	index = 0;
	uint32_t	i;
	for (i = 0; i < string.size(); i++)
	{
		if (string[i] == c && lock == true && i != index)
		{
			std::cout << "hola aqui estoy \n" << std::flush;
			finalString.push_back(string.substr(index, i - index));
			index = i + 1;
			lock = false;
		}
		if (string[i] != c)
			lock = true;
	}
	if (lock == true)
	{
		std::cout << "["<< i << "][" << string.substr(index, i - index) << "]\n";
		finalString.push_back(string.substr(index, i - index));
	}
	std::cout << "split end size = " << finalString.size() << '\n';
	return finalString;
}



int	main(int argc, char *argv[])
{
	std::string input = "1234567890 asdfghjklñ qwertyuiop";
    spliti(input, " ");
    return(0);
}
