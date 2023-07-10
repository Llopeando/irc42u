#include "../include/OperBlock.hpp"

OperBlock::OperBlock()
{
	
	std::vector<std::string> file = split(readFile(PATHFILE), '\n');
	for (std::vector<std::string>::iterator i = file.begin(); i != file.end(); i++)
	{
		std::vector<std::string>line = split(*i, '@');
		if (line.size() == 2)
			block[line[0]] = line[1];
	}
}

OperBlock::~OperBlock(){}

std::string OperBlock::getOperList()
{
	std::string list;
	for (std::unordered_map<std::string, std::string>::iterator i = block.begin(); i != block.end(); i++)
	{
		list += i->first;
		list += ", "; //si es el ultimo ????
	}
	return (list);
}

bool OperBlock::checkPass(const std::string &user, const std::string& pass) const
{
	std::unordered_map<std::string, std::string>::const_iterator it = block.find(user);
	if (it != block.end())
		return it->second == pass;
	
	return false;
}

bool OperBlock::findOper(const std::string &user)const
{
	return  block.find(user) != block.end();
}
