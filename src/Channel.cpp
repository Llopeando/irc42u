#include "../include/Channel.hpp"

START_SERVER_DATA_NAMESPACE

/* --- CONSTRUCTORS and DESTRUCTOR --- */

Channel::Channel(std::string name, std::string username, ServerData *data)
{
	this->name = name;
	this->topic = "";
	creator = username;
	this->data = data;
	users.resize(1);
	numOfUsers = 0;
	creationDate = utils::t_chrono::to_time_t(utils::t_chrono::now());
}

Channel::~Channel() { }

/* --- GETTER AND SETTERS --- */

std::string Channel::getCreator()const
{
	return creator;
}

std::string Channel::getCreationDate()const
{
	return std::to_string(creationDate);
}

std::string Channel::getName() const{
	return (name);
}

void Channel::addClient(clientIt index){
	users.push_back(index);
	numOfUsers++;
}

void Channel::setTopic(std::string topic){
	this->topic = topic;
}

std::string Channel::getTopic( void)const{
	return(topic);
}

uint32_t	Channel::getNumUser( void)const{
	return (users.size());

}

void	Channel::setCreationDate(std::time_t now){
	creationDate = now;
}

eFlags Channel::removeClient(clientIt indexAct){
	if (users.size() == 2) 
	{
		users.pop_back();
		return eRemoveChannel;
	}
	uint32_t user_pos = findUser(indexAct);
	if (user_pos == 0)
		return eSuccess;
	users[user_pos] = users[numOfUsers];
	numOfUsers--; 
	users.pop_back();
	return eSuccess;
}

/* --- OTHER CHANNELS UTILS FUNCTIONS --- */

uint32_t	Channel::findUser(clientIt indexAct) const {
	uint32_t i = 1;
	for (std::deque<uint32_t>::const_iterator it = users.begin() + 1; it != users.end(); it++)
	{
		if (*it == indexAct)
			return i;
		i++;
	}
	return (0);
}

void	Channel::shiftClients(clientIt index)
{
	for (std::deque<uint32_t>::iterator user = users.begin(); user != users.end();user++)
	{
		if (*user > index)
			(*user)--;
	}
}

END_SERVER_DATA_NAMESPACE