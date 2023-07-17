/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerData.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecamara <ecamara@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 12:46:49 by ecamara           #+#    #+#             */
/*   Updated: 2023/05/22 13:46:25 by ecamara          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ServerData.hpp"

ServerData::ServerData(ServerCreateInfo info): info(info)
{
	(void)this->info;
	IDs.resize(1);
	clientAddress.resize(1);
	clientAddressLength.resize(1);
	numOfClients = 0;
	memset(tempInfo.ports, 0, MAX_ROOMS);
	memset(tempInfo.ipAddresses, 0, sizeof(tempInfo.ipAddresses));
	rooms.reserve(info.numOfRooms);
	tempInfo.activeRooms = 0;
	for (uint8_t i = 0; i < info.numOfRooms;i++)
	{
		tempInfo.ports[i] = info.port + i + 1;
		rooms.push_back(ThreadedServer(info.port + (int)i + 1));
		tempInfo.activeRooms++;
	}
}

ServerData::~ServerData()
{
	
}

void	ServerData::runServer(uint8_t num)
{
	rooms[num].run();
}

const int	*ServerData::getPorts()const
{
	return tempInfo.ports;
}

const char	*ServerData::getIpAddresses()const
{
	return (const char *)tempInfo.ipAddresses;
}

uint8_t		ServerData::getActiveRooms()const
{
	return tempInfo.activeRooms;
}