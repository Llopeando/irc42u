/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecamara <ecamara@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 19:03:45 by ullorent          #+#    #+#             */
/*   Updated: 2023/04/13 20:01:48 by ecamara          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>

#include "defines.h"

class Client
{
	public:
		Client();
		~Client();

		uint8_t getState()const;
		void	setState(uint8_t state);
	
		uint32_t	getRole()const;
		void		setUsername(std::string username);
		std::string	getUsername()const;
		void		setNickname(std::string nickname);
		void		setPassword(std::string password);
		bool		checkPassword(std::string pass) const;
		uint32_t	getChannel()const;
		
	private:
		std::string nickname;
		std::string username;
		std::string password;
		uint8_t channel;
		uint8_t role;
		uint8_t state;
		bool	active;
		
};

#endif
