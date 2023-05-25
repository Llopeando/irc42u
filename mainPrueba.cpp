#include <ifaddrs.h>
#include <netinet/in.h>
#include <net/if.h>
#include <netdb.h>
#include <iostream>
#include <vector>

std::vector<std::string> split(const std::string &string, const char c)
{
	std::vector<std::string> finalString;
	bool		lock = false;
	uint32_t	index = 0;
	uint32_t	i;
	for (i = 0; i < string.size(); i++)
	{
		if (string[i] == c && lock == true && i != index)
		{
			//std::cout << "[" << i << "][" << string.substr(index, i - index) << "]\n" << std::flush;
			finalString.push_back(string.substr(index, i - index));
			index = i + 1;
			lock = false;
		}
		if (string[i] != c)
			lock = true;
	}
	if (lock == true)
	{
		//std::cout << "["<< i << "][" << string.substr(index, i - index) << "]\n";
		finalString.push_back(string.substr(index, i - index));
	}
	//std::cout << "split end size = " << finalString.size() << '\n';
	return finalString;
}

std::vector<std::string> splitIrcPrameters(const std::string &string, const char c)
{
	std::vector<std::string> finalString;
	bool		lock = false;
	uint32_t	index = 0;
	uint32_t	i;
	for (i = 0; i < string.size(); i++)
	{
		if (string[i] == ':' && i != 0) //Quitar :
		{
			lock = false;
			if (i - index > 0)
				finalString.push_back(string.substr(index, i - index));
			finalString.push_back(string.substr(i + 1, string.size() - i));
			return finalString;
		}
		if (string[i] == c && lock == true && i != index)
		{
			//std::cout << "[" << i << "][" << string.substr(index, i - index) << "]\n" << std::flush;
			finalString.push_back(string.substr(index, i - index));
			index = i + 1;
			lock = false;
		}
		if (string[i] != c)
			lock = true;
	}
	if (lock == true)
	{
		//std::cout << "["<< i << "][" << string.substr(index, i - index) << "]\n";
		finalString.push_back(string.substr(index, i - index));
	}
	//std::cout << "split end size = " << finalString.size() << '\n';
	return finalString;
}

int main()
{
	std::time_t date = 1225814334;
	std::cout << "DATE = [" << std::ctime(&date) << "]\n";
	return 0;
}