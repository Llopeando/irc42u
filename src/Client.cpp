/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecamara <ecamara@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 20:26:04 by ullorent          #+#    #+#             */
/*   Updated: 2023/04/11 18:35:27 by ecamara          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Client.hpp"

Client::Client()
{
	this->state = CL_STATE_SERVER_PASSWORD;
	this->active = CL_STATE_ACTIVE;
}

Client::~Client() {
	return ;
}

uint8_t	Client::getState() const
{
	return state;
}

void	Client::setState(uint8_t state)
{
	this->state = state;
}

std::string Client::getUsername()const
{ 
	return(username);
}


void Client::setUsername(std::string username)
{
	this->username = username;
}

void Client::setNickname(std::string nickname)
{
	this->nickname = nickname;
}

void Client::setPassword(std::string password) //ya veremos, esto no es muy seguro
{
	this->password = password;
}


bool Client::checkPassword(std::string pass)const {

	if (this->password == pass)
		return (true);
	else
		return (false);
}