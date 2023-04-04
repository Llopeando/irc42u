/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ullorent <ullorent@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 20:26:04 by ullorent          #+#    #+#             */
/*   Updated: 2023/04/04 16:51:46 by ullorent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Client.hpp"

Client::Client()
{
	this->state = CL_STATE_PASSWORD;
	this->active = CL_STATE_ACTIVE;
}

Client::~Client() {
	return ;
}

uint8_t	Client::getState()
{
	return state;
}

void	Client::setState(uint8_t state)
{
	this->state = state;
}