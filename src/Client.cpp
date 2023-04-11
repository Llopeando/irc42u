/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecamara <ecamara@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 20:26:04 by ullorent          #+#    #+#             */
/*   Updated: 2023/04/05 11:46:23 by ecamara          ###   ########.fr       */
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