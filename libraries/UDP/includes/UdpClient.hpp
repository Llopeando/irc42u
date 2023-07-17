/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UdpClient.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecamara <ecamara@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 10:44:40 by ecamara           #+#    #+#             */
/*   Updated: 2023/05/22 15:24:36 by ecamara          ###   ########.fr       */
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

#include "StaticColor.hpp"
#include "ServerDefines.hpp"
struct	UdpClientCreateInfo{
	int	port;
	std::string ip;
};

class UdpClient{
	public:
		UdpClient(UdpClientCreateInfo info);
		~UdpClient();

		ServerInfo	start();
		void		join(uint8_t serverIdx);
		void		send(char *buffer, uint32_t size);
	private:
		void	handShake();
		void	sendHandShake();
		void	confirmHandShake();
		void	receiveHandShake();
		void	finishHandShake();
		void	parseServerInfo(ServerInfo &info);
		void	sendInfoLoop();
		void	recvInfoLoop();

		void	sendData();
		
		bool		endThreads;
		std::mutex	endThreadsMutex;

		bool		dataToSend;
		char		*data;
		uint32_t	dataSize;
		std::mutex	sendMutex;

		int		socketFd;
		uint32_t myID;
		uint8_t serverIdx;
		
		ServerInfo serverInfo;
		struct	sockaddr_in RoomServerAddress;
		struct	sockaddr_in handShakeServerAddress;

		std::thread recvInfoThread;
		std::thread sendInfoThread;
};