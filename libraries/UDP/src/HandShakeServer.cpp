/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HandShakeServer.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecamara <ecamara@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 11:34:27 by ecamara           #+#    #+#             */
/*   Updated: 2023/05/22 13:44:01 by ecamara          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/HandShakeServer.hpp"

HandShakeServer::HandShakeServer(ServerCreateInfo info, UdpServer *server, ServerData *data) : server(server), data(data)
{
	IDs = 1;
	(void)this->server;
	socketFd = socket(AF_INET, SOCK_DGRAM, 0);
	if (socketFd == -1) {
		throw std::runtime_error("failed to create socket");
	}
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(info.port);  // Port number
	sockaddr.sin_addr.s_addr = INADDR_ANY;  // Bind to all available network interfaces
	if (bind(socketFd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) == -1) {
		close(socketFd);
		throw std::runtime_error("failed to bind socket");
	}
}

HandShakeServer::~HandShakeServer()
{
	close(socketFd);
}

void	HandShakeServer::run()
{
	while(1)
	{
		char buffer[sizeof(uint32_t)];  // Empty buffer
		struct sockaddr_in clientAddress;
		socklen_t clientAddressLength = sizeof(clientAddress);
		
		// Receive data from a client
		ssize_t bytesRead = recvfrom(socketFd, buffer, sizeof(buffer), 0,
									(struct sockaddr*)&clientAddress, &clientAddressLength);
		if (bytesRead < 0) {
			std::cerr << color::red << "Failed to receive data" << std::endl;
		}
		uint32_t ID;
		memcpy(&ID, buffer, sizeof(uint32_t));
		std::cout << color::green << "info received\n" << color::reset;
		if (ID == 0)
			initHandShake(clientAddress, clientAddressLength);
		else
			confirmHandShake(clientAddress, clientAddressLength, ID);
	}
}

void	HandShakeServer::initHandShake(sockaddr_in clientAddress, socklen_t clientAddressLength)
{
	ServerInfo sendInfo = {};
	memcpy(sendInfo.ipAddresses, data->getIpAddresses(), sizeof(sendInfo.ipAddresses));
	memcpy(sendInfo.ports, data->getPorts(), sizeof(sendInfo.ports));
	sendInfo.ID = IDs;
	sendInfo.activeRooms = data->getActiveRooms();
	std::cout << "send active rooms = " << (int)sendInfo.activeRooms << '\n';
	sendInfo.maxRooms = MAX_ROOMS;
	
	char *buffer[sizeof(ServerInfo)];
	memcpy(buffer, &sendInfo, sizeof(ServerInfo));
	ssize_t bytes =  sendto(socketFd, buffer, sizeof(ServerInfo), 0, (struct sockaddr*)&clientAddress, clientAddressLength);
	if (bytes == -1)
	{
		std::cerr << color::red << "Failed to send HandShake\n" << color::reset;
	}
	else
	{
		IDs++;
	}
}

void HandShakeServer::confirmHandShake(sockaddr_in clientAddress, socklen_t clientAddressLength, uint32_t ID)
{
	char buffer[sizeof(uint32_t)];
	memcpy(buffer, &ID, sizeof(buffer));
	ssize_t bytesSent = sendto(socketFd, buffer, sizeof(uint32_t), 0,
			(struct sockaddr*)&clientAddress, clientAddressLength);//init handshake
	if (bytesSent == -1)
	{
		std::cerr << color::red << "Failed to init handshake\n" << color::reset;
		exit(1);
	}
	std::cout << color::green << "handshake confirmed \n" << color::reset;
	memset(buffer, 0, sizeof(buffer));
	ssize_t bytesRead = recvfrom(socketFd, buffer, sizeof(buffer), 0,
									(struct sockaddr*)&clientAddress, &clientAddressLength);
	uint32_t newId;
	memcpy(&newId, buffer, bytesRead);
	if (newId < IDs && newId > 0)
		std::cout << color::green << "handShake finished\n" << color::reset;
	else
		std::cout << color::red << "handShake failed\n" << newId  << "\n" << color::reset;
}

void HandShakeServer::sendMessage(sockaddr_in clientAddress, socklen_t clientAddressLength, const char *message)
{
	size_t messageLength = strlen(message);  // Length of the message
	
	// Send the message back to the client
	ssize_t bytesSent = sendto(socketFd, message, messageLength, 0,
								(struct sockaddr*)&clientAddress, clientAddressLength);
	if (bytesSent == -1) {
		std::cerr << "Failed to send messsage\n";
	}
}