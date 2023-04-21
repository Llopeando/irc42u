/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecamara <ecamara@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 20:30:41 by ullorent          #+#    #+#             */
/*   Updated: 2023/04/13 18:50:221 by ecamara          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Channel.hpp"

/* --- CONSTRUCTORS and DESTRUCTOR --- */
Channel::Channel(std::string name, std::string username, std::deque<struct pollfd> *pollfds, std::deque<t_activeIndex> *actives, std::deque<Client> *registered)
{
	this->name = name;
	creator = username;
	this->pollfds = pollfds;
	this->actives = actives;
	this->registered = registered;

}

Channel::~Channel() {
	
}

/* --- GETTER --- */
std::string	const Channel::getName() const{
	return (name);
}

void Channel::addClient(uint32_t indexAct){
	users.push_back(indexAct);
	uint32_t user_pos = users.size() - 1;
	std::string msgUser = color::boldyellow + "ℹ️  You are in the " + color::boldyellow + this->name + color::boldgreen + " channel\n" + color::reset;
	std::string msgChannel = "\nℹ️  "  + (*registered)[(*actives)[indexAct].index].getUsername() + color::boldgreen +  " joined the channel\n" + color::reset;
	(*registered)[(*actives)[indexAct].index].resetLastMsgIdx(); //resetea el index de log 

	sendMsgUser((*pollfds)[indexAct].fd, msgUser);
	sendMsgChannel(user_pos, msgChannel);
}

/* ------------------------------------------------------------ */
/*					SEND MESSAGES IN CHANNEL					*/
/* ------------------------------------------------------------ */
//de momento va duplicado, uno en channel y otro en server 

void	Channel::sendMsgUser(uint32_t user_pos, std::string const &str) const {
	int buffer_size = 65536;      //en defines?

	setsockopt((*pollfds)[users[user_pos]].fd, SOL_SOCKET, SO_SNDBUF, &buffer_size, sizeof(buffer_size));
	send((*pollfds)[users[user_pos]].fd, str.c_str(), str.size(), 0);
}

void	Channel::sendMsgChannel(uint32_t user_pos, std::string const &str){

	msg_log.push_back(str);
	std::string msg = "\033[1;37m<" + (*registered)[(*actives)[users[user_pos]].index].getUsername() + ">\033[0m " + str;
	//sendMsgUser((*pollfds)[users[user_pos]].fd, color::white + "<" + (*registered)[(*actives)[users[user_pos]].index].getUsername() + ">" + color::reset);
	
	for(uint32_t i = 0; i < users.size();i++)
	{
		std::cout << color::white << (*registered)[(*actives)[users[i]].index].getUsername() << " ----- " << ((*registered)[(*actives)[users[i]].index].getInputBlock()) << '\n' ;
		if (i != user_pos && !((*registered)[(*actives)[users[i]].index].getInputBlock()))
		{
			sendMsgUser(i, msg);
			(*registered)[(*actives)[users[user_pos]].index].addLastMsgInx(1);
		}
	}
	
}

uint32_t	Channel::findUser(uint32_t indexAct)const {
	for (uint32_t i = 0; i < users.size();i++)
	{
		if (users[i] == indexAct)
			return i;
	}
	return (-1);
}

void	Channel::broadcast(uint32_t indexAct, std::string const &msg) {
	uint32_t user_pos;

	user_pos = findUser(indexAct);
	if (user_pos > users.size())
		return ;
	//std::cout << (*registered)[(*actives)[indexAct].index].getUsername() << " writes: " << msg << '\n' ;
	sendMsgChannel(user_pos, msg);
}

	void Channel::refresh(uint32_t indexAct)
	{
		//uint32_t user_pos = findUser(indexAct);
		for (uint32_t i = (*registered)[(*actives)[indexAct].index].getLastMsgIdx();i < msg_log.size(); i++)
		{
			sendMsgUser((*pollfds)[indexAct].fd, msg_log[i]);
		}
	}
