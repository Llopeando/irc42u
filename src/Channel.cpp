/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ullorent <ullorent@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 20:30:41 by ullorent          #+#    #+#             */
/*   Updated: 2023/03/24 20:30:54 by ullorent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(std::string name, std::deque<struct pollfd> *pollfds)
{
	this->name = name;
	this->pollfds = pollfds;
}

Channel::~Channel() {
	
}

const char	*Channel::getName() {
	return (name.c_str());
}