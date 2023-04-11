/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecamara <ecamara@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 19:03:45 by ullorent          #+#    #+#             */
/*   Updated: 2023/04/05 11:45:42 by ecamara          ###   ########.fr       */
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

		uint8_t getState()const;
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
