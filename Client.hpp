/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ullorent <ullorent@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 19:03:45 by ullorent          #+#    #+#             */
/*   Updated: 2023/03/24 20:28:54 by ullorent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include "irc.h"

class Client
{
	public:
		Client(/* args */);
		~Client();

		uint8_t getState();
		void	setState(uint8_t state);
	private:
		std::string nickname;
		std::string username;
		//uint8_t channel;
		//uint8_t role;
		uint8_t state;
		bool	active;
};

#endif
