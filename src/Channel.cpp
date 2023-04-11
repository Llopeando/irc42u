/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecamara <ecamara@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 20:30:41 by ullorent          #+#    #+#             */
/*   Updated: 2023/04/05 11:45:09 by ecamara          ###   ########.fr       */
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

const char	*Channel::getName() const{
	return (name.c_str());
}