/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ThreadedServer.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecamara <ecamara@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 11:58:10 by ecamara           #+#    #+#             */
/*   Updated: 2023/05/24 16:59:06 by ecamara          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ThreadedServer.hpp"

ThreadedServer::ThreadedServer(int port) : port(port)
{
	endThreads = false;

	dataToSend = false;

	(void)this->port;
	socketFd = socket(AF_INET, SOCK_DGRAM, 0);
	if (socketFd == -1) {
		throw std::runtime_error("failed to create socket");
	}
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(port);  // Port number
	sockaddr.sin_addr.s_addr = INADDR_ANY;  // Bind to all available network interfaces

	if (bind(socketFd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) == -1) {
		close(socketFd);
		throw std::runtime_error("failed to bind socket");
	}
}
ThreadedServer::~ThreadedServer()
{
	endThreadsMutex.lock();
	endThreads = true;
	endThreadsMutex.unlock();
	recvInfoThread.join();
	sendInfoThread.join();
}

void	ThreadedServer::run()
{
	recvInfoThread = std::thread([this](){
		this->sendInfoLoop();
	});
	sendInfoThread = std::thread([this](){
		this->recvInfoLoop();
	});
	while(1)
	{
		
	}
}

void	ThreadedServer::sendInfoLoop()
{
	while(1)
	{
		endThreadsMutex.lock();
		if (endThreads)
			break ;
		endThreadsMutex.unlock();
	}
}

void	ThreadedServer::recvInfoLoop()
{
	while(1)
	{
		endThreadsMutex.lock();
		if (endThreads)
			break ;
		endThreadsMutex.unlock();
	}
}

void	ThreadedServer::send(char *buffer, uint32_t size)
{
	sendMutex.lock();
	data = buffer;
	dataSize = size;
	dataToSend = true;
	sendMutex.unlock();
}

void	ThreadedServer::sendData()
{
	sendMutex.lock();
	//if (dataToSend)
	//{
	//	socklen_t serverAddressLength = sizeof(RoomServerAddress);
	//	ssize_t bytesSent = sendto(socketFd, data, dataSize, 0,
	//			(struct sockaddr*)&RoomServerAddress, serverAddressLength);//init handshake
	//	if (bytesSent == -1)
	//	{
	//		std::cerr << color::red << "Failed to init handshake\n" << color::reset;
	//	}
	//}
	sendMutex.unlock();
}