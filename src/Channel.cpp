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
Channel::Channel(std::string name, std::string username, UsersData *data)
{
	this->name = name;
	creator = username;
	this->data = data;
	users.resize(0);
	numOfUsers = 0;
}

Channel::~Channel() {
	
}

/* --- GETTER --- */
std::string	const Channel::getName() const{
	return (name);
}

void Channel::addClient(uint32_t indexAct){
	users.push_back(indexAct);
	uint32_t user_pos = numOfUsers;
	numOfUsers++;
	std::string infoUser = color::boldyellow + "ℹ️  You are in the " + color::boldyellow + this->name + color::boldgreen + " channel\n\n" + color::reset;
	std::string infoMsg = color::boldgreen +  " joined the channel\n" + color::reset;
	(*data)[indexAct].resetLastMsgIdx(); //resetea el index de log 
	sendMsgUser(user_pos, infoUser);
	sendMsgChannel(user_pos, infoMsg);
}

/* ------------------------------------------------------------ */
/*					SEND MESSAGES IN CHANNEL					*/
/* ------------------------------------------------------------ */
//de momento va duplicado, uno en channel y otro en server 

void	Channel::sendMsgUser(uint32_t user_pos, std::string const &str) const {
	int buffer_size = 65536;      //en defines?
	setsockopt(data->getFd(users[user_pos]), SOL_SOCKET, SO_SNDBUF, &buffer_size, sizeof(buffer_size));
	send(data->getFd(users[user_pos]), str.c_str(), str.size(), 0);
}

void Channel::flushLog(Client &user, uint32_t user_pos){

	//check last msg
	uint32_t i;
	for (i = user.getLastMsgIdx(); i < msg_log.size(); i++)
		sendMsgUser(user_pos, "\t\t\t" + msg_log[i]);
	i -= user.getLastMsgIdx();
	user.addLastMsgIdx(i);
}
/*
void	Channel::sendInfoChannel(uint32_t user_pos, std::string const &str)
{
	flushLog((*data)[users[user_pos]], user_pos);
	for(uint32_t i = 0; i < users.size();i++)
	{
		if (!((*data)[users[i]].getInputBlock())) // solo en fiuncion de visualizacion
		{
			sendMsgUser(i, msg);
			(*data)[users[i]].addLastMsgIdx(1);
		}
	}
}*/

void	Channel::sendMsgChannel(uint32_t user_pos, std::string const &str)
{
	std::string msg = color::cyan + "<" + (*data)[users[user_pos]].getUsername() + "> " + color::boldwhite + str + color::reset;

	msg_log.push_back(msg);
	flushLog((*data)[users[user_pos]], user_pos);
	for(uint32_t i = 0; i < users.size();i++)
	{
		if (!((*data)[users[i]].getInputBlock())) // solo en fiuncion de visualizacion
		{
			sendMsgUser(i, msg);
			(*data)[users[i]].addLastMsgIdx(1);
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
	uint32_t user_pos = findUser(indexAct);
	flushLog((*data)[indexAct], user_pos);
}
