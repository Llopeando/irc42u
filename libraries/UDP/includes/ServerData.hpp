/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerData.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecamara <ecamara@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 12:40:00 by ecamara           #+#    #+#             */
/*   Updated: 2023/05/19 17:02:23 by ecamara          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <vector>
#include <sys/socket.h>//socklen_t
#include <netinet/in.h>//sockaddr_in
#include "ServerDefines.hpp"
#include "ThreadedServer.hpp"
#include "StaticColor.hpp"

class ServerData{
	public:
		ServerData(ServerCreateInfo info);
		~ServerData();

		void		runServer(uint8_t num);
		const int	*getPorts()const;
		const char	*getIpAddresses()const;
		uint8_t		getActiveRooms()const;
	private:
		ServerCreateInfo info;
		uint32_t numOfClients;
		std::vector<uint32_t>IDs;//0 is reserved
		std::vector<sockaddr_in>clientAddress;//0 is reserved
		std::vector<socklen_t>clientAddressLength;//0 is reserved
		std::vector<ThreadedServer> rooms;
		ServerInfo tempInfo;
};