#include <iostream>
#include "include/defines.h"
#include <vector>
#include <string>


uint32_t countWords(const char *str, char c)
{
	uint32_t	size = strlen(str);
	uint32_t	count = 1;
	bool		lock = false;
	for(uint32_t i = 0; i < size; i++)
	{
		if (str[i] == c)
		{
			lock = true;
		}
		else if (lock == true)
		{
			count++;
			lock = false;
		}
	}
	return count;
}

std::vector<std::string> split(const std::string &string, const char c)
{
	uint32_t	wordCount = countWords(string.c_str(), c);
	std::vector<std::string> finalString(wordCount);
	bool		lock = false;
	uint32_t	index = 0;
	uint32_t i;
	for (i = 0; i < string.size(); i++)
	{
		if (string[i] == c && lock == true && i != index)
		{
			std::cout << "["<< i << "][" << string.substr(index, i - index) << "]\n";
			finalString.push_back(string.substr(index, i - index));
			index = i+1;
			lock = false;
		}
		if (string[i] != c)
			lock = true;
	}
	if (lock == true)
		finalString.push_back(string.substr(index, i - index));
	return finalString;
}



int main()
{
	printIp();
	std::string name = "my name is eneko";
	std::vector<std::string> words = split(name, ' ');
	for (auto word : words)
	{
		std::cout << "[" << word << "]\n";
	}
	return 0;
}