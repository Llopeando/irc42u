/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ullorent <ullorent@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 17:44:08 by ullorent          #+#    #+#             */
/*   Updated: 2023/08/03 18:13:18 by ullorent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "defines.h"


#include "Utils.hpp"
#include "ServerDataStructs.h"

#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <poll.h>
#include <deque>

START_SERVER_DATA_NAMESPACE

class ServerData;

class Channel
{
	public:
		Channel(std::string name, std::string username, ServerData *data);
		~Channel();

		uint32_t operator[](uint32_t idx)const{return users[idx];}
		uint32_t operator[](uint32_t idx){return users[idx];}

		std::string			getCreator() const;
		std::string			getCreationDate()const;
		void				setCreationDate(std::time_t);
		std::string			getName()const;
		void				addClient(clientIt index);
		eFlags				removeClient(clientIt indexAct);
		void				setTopic(std::string topic);
		std::string			getTopic( void)const;
		uint32_t			getNumUser( void)const;
		uint32_t			findUser(clientIt indexAct)const;
		void				shiftClients(clientIt index);

	private:

		void	sendInfoChannel(uint32_t user_pos, std::string const &str);
		uint32_t numOfUsers;
		ServerData *data;
		std::string name;
		std::string topic;
		std::deque<uint32_t> users;
		std::string creator;
		std::time_t creationDate;
};

END_SERVER_DATA_NAMESPACE

#endif