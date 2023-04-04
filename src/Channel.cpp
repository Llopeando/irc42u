/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ullorent <ullorent@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 20:30:41 by ullorent          #+#    #+#             */
/*   Updated: 2023/04/04 17:23:37 by ullorent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Channel.hpp"

Channel::Channel(std::string name, std::deque<struct pollfd> *pollfds)
{
	(void)pollfds;
	this->name = name;
	//this->pollfds = pollfds;
}

Channel::~Channel() {
	
}

const char	*Channel::getName() {
	return (name.c_str());
}