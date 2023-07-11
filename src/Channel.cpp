/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ullorent <ullorent@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 14:00:32 by ullorent          #+#    #+#             */
/*   Updated: 2023/07/11 17:32:11 by ullorent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Channel.hpp"
#include "../include/Client.hpp"

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

Channel::~Channel() {
	
}

/* --- GETTER AND SETTERS --- */

std::string Channel::getCreator()const
{
	return creator;
}

char	*Channel::getCreationDate()const
{
	return std::ctime(&creationDate);
}

std::string Channel::getName() const{
	return (name);
}

void Channel::addClient(clientIt index){
	users.push_back(index);
	//uint32_t user_pos = numOfUsers;
	numOfUsers++;
}

void Channel::setTopic(std::string topic){
	this->topic = topic;
}

std::string Channel::getTopic( void)const{
	return(topic);
}

uint32_t	Channel::getNumUser( void)const{
	return (numOfUsers);

}

void	Channel::setCreationDate(std::time_t now){
	creationDate = now;
}



void Channel::removeClient(clientIt indexAct){
	
	//quitarlo del array
	uint32_t	user_pos = findUser(indexAct);
	if (user_pos == 0)
	{
		return;
	}
	numOfUsers--;
	users[user_pos] = users[numOfUsers];
	users.pop_back();
	}


uint32_t	Channel::findUser(clientIt indexAct)const {
	for (uint32_t i = 1; i < users.size();i++)
	{
		if (users[i] == indexAct)
			return i;
	}
	return (0);
}

void	Channel::broadcast(clientIt sender, std::string const &msg) {
	for(clientIt i = 1; i < users.size();i++)
	{
		if (users[i] != sender)
		{
			//std::cout << color::green << "SENDED TO: [" << i << "]" << (*data)[(clientIt)users[i]].getUsername() << "\n" << color::reset;
			utils::sendMsgUser((*data)[(pollfdIt)users[i]].fd, msg);
		}
	}
}

std::string Channel::getUserList()const
{
	std::string result;
	for (std::deque<uint32_t>::const_iterator it = users.begin() + 1; it != users.end(); it++)
	{
		result += (*data)[(clientIt)(*it)].getNickname() + " ";
	}
	return result;
}

END_SERVER_DATA_NAMESPACE