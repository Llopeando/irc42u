/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ullorent <ullorent@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 17:44:08 by ullorent          #+#    #+#             */
/*   Updated: 2023/07/27 17:44:11 by ullorent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Config.h"
#include "defines.hpp"
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

		std::string			getCreator() const;
		char				*getCreationDate()const;
		void				setCreationDate(std::time_t);
		std::string			getName()const;
		std::string			getUserList()const;
		void				addClient(clientIt index);
		void				removeClient(clientIt indexAct);
		void 				broadcast(clientIt sender, std::string const &msg);
		//void				refresh(uint32_t indexAct);
		//void				flushLog(Client &user, uint32_t user_pos);
		void				setTopic(std::string topic);
		std::string			getTopic( void)const;
		uint32_t			getNumUser( void)const;
		uint32_t			findUser(clientIt indexAct)const;

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

#include "ServerData.hpp"

#endif