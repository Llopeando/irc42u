/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecamara <ecamara@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 20:26:04 by ullorent          #+#    #+#             */
/*   Updated: 2023/04/28 20:50:30 by ecamara          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Client.hpp"

/* --- CONSTRUCTORS and DESTRUCTOR --- */
Client::Client(){
	state = CL_STATE_SERVER_PASSWORD;
	active = CL_STATE_ACTIVE;
	channel = 0;
	inputBlock = 1;
	lastMsgIndex = 0;
	newClient = 1;
}

Client::Client(const Client &client)
{
	*this = client;
}

Client::~Client() {
	return ;
}

Client& Client::operator=(const Client& client) {
	if (this != &client) {
	this->nickname = client.nickname;
	this->username = client.username;
	this->password = client.password;
	this->lastMsgIndex = client.lastMsgIndex;
	this-> channel = client.channel;
	this-> role = client.role;
	this-> state = client.state;
	this->active = client.active;
	this->inputBlock = client.inputBlock;
    }
    return *this;
}



/* --- DATA SETTERS --- */
uint8_t	Client::getState() const {
	return state;
}

void	Client::setState(uint8_t state) {
	this->state = state;
}

void Client::setUsername(std::string username) {
	this->username = username;
}

void Client::setNickname(std::string nickname) {
	this->nickname = nickname;
}

void Client::setPassword(std::string password) { //ya veremos, esto no es muy seguro
	this->password = password;
}

/* --- DATA GETTERS --- */
std::string	Client::getUsername() const {
	return (username);
}

uint32_t	Client::getChannel() const {
	return (channel);
}

uint32_t	Client::getRole()const
{
	return (role);
}

void Client::setRole(uint8_t role)
{
	this->role = role;
}

bool Client::getNewClient() const{
	return(newClient);
}

void Client::setNewClient(bool check) {
	newClient = check;
}


/* --- CHECKER FUNCTIONS --- */
bool Client::checkPassword(std::string pass)const {

	if (this->password == pass)
		return (true);
	else
		return (false);
}

uint32_t Client::getLastMsgIdx()const{
	return(lastMsgIndex);
}

bool Client::getInputBlock()const
{
	return inputBlock;
}

void Client::addLastMsgIdx(uint32_t num)
{
	lastMsgIndex += num;
}

void Client::resetLastMsgIdx(){ lastMsgIndex = 0; }

void Client::setLastMsgIdx(uint32_t msgIdx)
{
	lastMsgIndex = msgIdx;
}