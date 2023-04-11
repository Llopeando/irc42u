/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecamara <ecamara@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 19:03:45 by ullorent          #+#    #+#             */
/*   Updated: 2023/04/11 19:02:08 by ecamara          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#define SERVER_FAILURE -1
#define CL_STATE_SERVER_PASSWORD 0
#define CL_STATE_LOG_IN 1
#define CL_STATE_SELECT_USERNAME 2
#define CL_STATE_INTRODUCE_PERSONAL_PASSWORD 3
#define CL_STATE_SELECT_NICKNAME 4
#define CL_STATE_LOBBY 5
#define CL_STATE_IN_CHANNEL 6

#define CL_STATE_ACTIVE 1
#define CL_STATE_INACTIVE 0

class Client
{
	public:
		Client(/* args */);
		~Client();

		uint8_t getState()const;
		void	setState(uint8_t state);
	
		void		setUsername(std::string username);
		std::string	getUsername()const;
		void		setNickname(std::string nickname);
		void		setPassword(std::string password);
		bool		checkPassword(std::string pass) const;
		
	private:
		std::string nickname;
		std::string username;
		std::string password;
		//uint8_t channel;
		//uint8_t role;
		uint8_t state;
		bool	active;
		
};

#endif
