#include "../includes/UdpServer.hpp"

UdpServer::UdpServer(ServerCreateInfo info):info(info)
{
}

UdpServer::~UdpServer()
{

}

void UdpServer::runHandShakeServer()
{
	handShakeServer.run();
}

void UdpServer::runServer(uint8_t num)
{
	if (num < info.numOfRooms)
		data.runServer(num);
}
const char	* UdpServer::getIpAddresses()const
{
	return data.getIpAddresses();
}

const int	*UdpServer::getPorts()const
{
	return data.getPorts();
}

uint8_t UdpServer::getActiveRooms()const
{
	return data.getActiveRooms();
}