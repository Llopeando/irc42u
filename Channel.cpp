#include "Channel.hpp"


Channel::Channel(std::string name, std::deque<struct pollfd> *pollfds)
{
	this->name = name;
	this->pollfds = pollfds;
}