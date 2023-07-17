/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HandShakeServer.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecamara <ecamara@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 11:16:48 by ecamara           #+#    #+#             */
/*   Updated: 2023/05/19 14:50:49 by ecamara          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <vector>

#include "StaticColor.hpp"

class UdpServer;
class ServerData;

#include "ServerDefines.hpp"

class HandShakeServer{
	public:
		HandShakeServer(ServerCreateInfo info, UdpServer *server, ServerData *data);
		~HandShakeServer();
		void run();
	private:
		void	initHandShake(sockaddr_in clientAddress, socklen_t clientAddressLength);
		void	sendMessage(sockaddr_in clientAddress, socklen_t clientAddressLength, const char *message);
		void	confirmHandShake(sockaddr_in clientAddress, socklen_t clientAddressLength, uint32_t ID);

		int			socketFd;
		sockaddr_in	sockaddr;
		uint32_t	IDs;//0 reserved
		UdpServer	*server;
		ServerData		*data;
};

#include "UdpServer.hpp"
#include "ServerData.hpp"