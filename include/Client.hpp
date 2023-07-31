/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ullorent <ullorent@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 17:44:06 by ullorent          #+#    #+#             */
/*   Updated: 2023/07/28 18:42:10 by ullorent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>

#include "defines.h"
#include "ServerDataStructs.h"

class Client
{
	public:
		Client();
		Client(std::string hostname);
		Client(const Client &client);
		~Client();
		
		void addBuffer(const std::string& str);
		std::string getBuffer() const;
		void emptyBuffer();

		void		addChannelToOps(sd::channIt index);
		uint32_t	getChannelToOps()const;
		bool		findChannelInOps(sd::channIt index)const;
		void		updateOps(uint32_t client);

		Client& operator=(const Client& client);
		std::string	getUserMask()const;
		std::string	getUsername()const;
		std::string getHostname()const;
		std::string	getAwayMsg()const;
		std::string	getNickname()const;
		bool		getAwayStatus() const;
		int			getAuthentificied()const;
		uint8_t		getState() const;
		uint32_t	getRole()const;

		void		setAuthentificied(int status);
		void		setRole(uint8_t role);
		void		setAwayStatus(bool isAway);
		void		setAwayMsg(std::string away_msg);
		void		setUsername(std::string username);
		void		setNickname(std::string nickname);
		void		setPassword(std::string password);
		
		bool		checkPassword(std::string pass) const;
		
	private:
		std::string nickname;
		std::string username;
		std::string password;
		std::string away_msg;
		std::string hostname;

		std::string buffer;
		uint32_t channelOps;
		uint8_t role;
		uint8_t state;
		bool	isAway;
		int	authentificied;
		//bool	inputBlock
};

#endif
