/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ThreadedServer.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecamara <ecamara@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 11:18:01 by ecamara           #+#    #+#             */
/*   Updated: 2023/05/22 16:22:01 by ecamara          ###   ########.fr       */
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
#include <exception>
#include <thread>

#include "StaticColor.hpp"

class ThreadedServer{
	public:
		ThreadedServer(int port);
		~ThreadedServer();
		void	run();
		void	send(char *buffer, uint32_t size);
	private:
		void	sendInfoLoop();
		void	recvInfoLoop();

		void	sendData();

		bool		endThreads;
		std::mutex	endThreadsMutex;

		bool		dataToSend;
		char		*data;
		uint32_t	dataSize;
		std::mutex	sendMutex;

		std::thread recvInfoThread;
		std::thread sendInfoThread;


		int	port;
		int	socketFd;
		sockaddr_in	sockaddr;
};