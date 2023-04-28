/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UsersData.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecamara <ecamara@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 16:36:43 by ecamara           #+#    #+#             */
/*   Updated: 2023/04/28 20:29:13 by ecamara          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USERSDATA_HPP
#define USERSDATA_HPP

#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>


#include "Client.hpp"
#include "defines.h"


class UsersData{
	public:
		UsersData();
		~UsersData();
		void newTempUser(int fd); 
		void newRegisteredUser(uint32_t indexAct);
		Client& operator[](uint32_t indexAct); //override de operador [] para acceder al cliente, ya sea temporal o registrado, le pasamos el activeindex
		const Client& operator[](uint32_t indexAct) const;
		
		uint32_t findUsername(const std::string &username) const;
		
		int		getFd(uint32_t indexAct)const;
		void	popTemp(uint32_t indexAct);
		
		void findTmpIndexAndRest(uint32_t indexTemp, uint32_t num);
		
		std::vector<struct pollfd>&	Pollfd();
		std::vector<t_activeIndex>&	Actives();
		std::vector<Client>&	Registered();
		std::vector<Client>&	TempClients();
		
		
		
		
		uint32_t	sizeAct();
		uint32_t	sizeReg();
		uint32_t	sizeTmp();
	private:
		std::vector<struct pollfd> pollfds;
		std::vector<t_activeIndex> actives;
		std::vector<Client> tempClients;
		std::vector<Client> registered;
};

#endif