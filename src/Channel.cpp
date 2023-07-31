/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ullorent <ullorent@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 14:00:32 by ullorent          #+#    #+#             */
/*   Updated: 2023/07/31 12:53:00 by ullorent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	//std::cout << "CH NAME: "<< name << " DATE: " << std::ctime(&creationDate) << "]\n";
}

Channel::~Channel() {
	//std::cout << "CHANNEL DESTROYED\n";
}

/* --- GETTER AND SETTERS --- */

std::string Channel::getCreator()const
{
	return creator;
}

std::string Channel::getCreationDate()const
{
	//return std::ctime(&creationDate);
	return std::to_string(creationDate);
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

eFlags Channel::removeClient(clientIt indexAct){
	if (users.size() == 1)
	{
		return eRemoveChannel;
	}
	uint32_t	user_pos = findUser(indexAct);
	if (user_pos == 0)
	{
		return eSuccess;
	}
	numOfUsers--;
	if (numOfUsers == 0)
	{
		users.pop_back();
		//data->deleteChannel(data->findChannel(name));
		return eRemoveChannel;
	}
	users[user_pos] = users[numOfUsers];
	users.pop_back();
	return eSuccess;
}

uint32_t	Channel::findUser(clientIt indexAct) const {
	uint32_t i = 1;
	for (std::deque<uint32_t>::const_iterator it = users.begin() + 1; it != users.end(); it++)
	{
		std::cout << "i[" << i << "]size[" << users.size() << "]\n";
		if (*it == indexAct)
			return i;
		i++;
	}
	return (0);
}



END_SERVER_DATA_NAMESPACE