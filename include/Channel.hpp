/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecamara <ecamara@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 16:47:46 by ecamara           #+#    #+#             */
/*   Updated: 2023/04/21 18:52:40 by ecamara          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "defines.h"


#include <iostream>
//#include <cstring>
//#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
//#include <cstdio>
#include <poll.h>
#include <deque>
#include "Client.hpp"

typedef struct activeIndex
{
	bool		registered;
	uint32_t	index;
}t_activeIndex;


class Channel
{
	public:
		Channel(std::string name, std::string username, std::deque<struct pollfd> *pollfds, std::deque<t_activeIndex>*actives, std::deque<Client> *registered);
		~Channel();
		std::string const 	getName()const;
		void 				addClient(uint32_t indexAct);
		void 				broadcast(uint32_t indexAct, std::string const &msg);
		void				refresh(uint32_t indexAct);
	private:
		std::string name;
		std::deque<uint32_t> users;
		//std::deque<uint32_t>msgIndexUsr; //ultimo mensaje leido
		std::string creator;
		std::deque<std::string> msg_log;
		
		//Punteros a arrays de server
		std::deque<struct pollfd> *pollfds; 
		std::deque<Client> *registered;
		std::deque<t_activeIndex> *actives;

		uint32_t	findUser(uint32_t indexAct)const;
		void		sendMsgUser(uint32_t user_pos, std::string const &str) const;
		void		sendMsgChannel(uint32_t indexAct, std::string const &str);
};

#endif