#include "../includes/UdpClient.hpp"

UdpClient::UdpClient(UdpClientCreateInfo info)
{
	endThreads = false;
	dataToSend = false;

	socketFd = socket(AF_INET, SOCK_DGRAM, 0);
	if (socketFd == -1) {
		throw std::runtime_error("failed to create socket");
	}

	handShakeServerAddress.sin_family = AF_INET;
	handShakeServerAddress.sin_port = htons(info.port);
	inet_pton(AF_INET, info.ip.c_str(), &(handShakeServerAddress.sin_addr));
}

UdpClient::~UdpClient()
{
	endThreadsMutex.lock();
	endThreads = true;
	endThreadsMutex.unlock();
	recvInfoThread.join();
	sendInfoThread.join();
	close(socketFd);	
}

ServerInfo UdpClient::start()
{
	handShake();
	recvInfoThread = std::thread([this](){
		this->recvInfoLoop();
	});
	sendInfoThread = std::thread([this](){
		this->sendInfoLoop();
	});
	return serverInfo;
	//std::thread sendInfoThread([this](){
	//	this->sendInfoLoop();
	//});
}

void UdpClient::handShake()
{
	sendHandShake();
	receiveHandShake();
	confirmHandShake();
	finishHandShake();
}

void UdpClient::sendHandShake()
{
	char buffer[sizeof(uint32_t)];
	memset(buffer, 0, sizeof(buffer));
	ssize_t bytesSent = sendto(socketFd, buffer, sizeof(uint32_t), 0,
			(struct sockaddr*)&handShakeServerAddress, sizeof(handShakeServerAddress));//init handshake
	if (bytesSent == -1)
	{
		std::cerr << color::red << "Failed to init handshake\n" << color::reset;
		exit(1);
	}
	std::cout << color::green << "handshake initiated\n" << color::reset;
}

void	UdpClient::receiveHandShake()
{
	char buffer[sizeof(ServerInfo)];
	socklen_t serverAddressLength = sizeof(handShakeServerAddress);
	ssize_t bytesRead = recvfrom(socketFd, buffer, sizeof(buffer), 0,
				(struct sockaddr*)&handShakeServerAddress, &serverAddressLength);
	if (bytesRead == -1)
	{
		std::cerr << color::red << "Failed to receive handshake\n" << color::reset;
		exit(1);
	}
	ServerInfo info = {};
	memcpy(&info, buffer, sizeof(ServerInfo));
	parseServerInfo(info);
}

void	UdpClient::parseServerInfo(ServerInfo &info)
{
	serverInfo = info;
	myID = info.ID;
	std::cout << color::boldgreen << "MY ID is: " << info.ID << "\n" << color::reset;
}

void	UdpClient::confirmHandShake()
{
	char buffer[sizeof(uint32_t)];
	memcpy(buffer, &myID, sizeof(buffer));
	ssize_t bytesSent = sendto(socketFd, buffer, sizeof(uint32_t), 0,
			(struct sockaddr*)&handShakeServerAddress, sizeof(handShakeServerAddress));//init handshake
	if (bytesSent == -1)
	{
		std::cerr << color::red << "Failed to init handshake\n" << color::reset;
		exit(1);
	}
	std::cout << color::green << "handshake confirmed \n" << color::reset;
}

void	UdpClient::finishHandShake()
{
	char buffer[sizeof(uint32_t)];
	socklen_t serverAddressLength = sizeof(handShakeServerAddress);
	ssize_t bytesRead = recvfrom(socketFd, buffer, sizeof(buffer), 0,
				(struct sockaddr*)&handShakeServerAddress, &serverAddressLength);
	if (bytesRead == -1)
	{
		std::cerr << color::red << "Failed to receive handshake\n" << color::reset;
		exit(1);
	}
	uint32_t checkId;
	memcpy(&checkId, buffer, sizeof(uint32_t));
	if (checkId == myID)
		std::cout << color::green << "ID correctly set\n" << color::reset;
	else
	{
		std::cerr << color::red << "ID's do not match\n" << color::reset;
		exit(1);
	}
	ssize_t bytesSent = sendto(socketFd, buffer, sizeof(uint32_t), 0,
			(struct sockaddr*)&handShakeServerAddress, sizeof(handShakeServerAddress));//init handshake
	if (bytesSent == -1)
	{
		std::cerr << color::red << "Failed to init handshake\n" << color::reset;
		exit(1);
	}
	std::cout << color::green << "handshake finished \n" << color::reset;
	serverInfo.print();
}

void	UdpClient::join(uint8_t serverIdx)
{
	if (serverIdx > serverInfo.activeRooms)
		return ;
	this->serverIdx = serverIdx;
	handShakeServerAddress.sin_family = AF_INET;
	handShakeServerAddress.sin_port = htons(serverInfo.ports[serverIdx]);
	char	ipAddress[15];
	memcpy(ipAddress, serverInfo.ipAddresses + serverIdx * 15, 15);
	inet_pton(AF_INET, ipAddress, &(handShakeServerAddress.sin_addr));
}

void	UdpClient::recvInfoLoop()
{
	while(1)
	{
		sendData();
		endThreadsMutex.lock();
		if (endThreads == true)
			break ;
		endThreadsMutex.unlock();
		std::cout << "\r";
	}
}

void	UdpClient::sendInfoLoop()
{
	while (1)
	{
		endThreadsMutex.lock();
		if (endThreads == true)
			break ;
		endThreadsMutex.unlock();
		std::cout << "\r";
	}
}

void	UdpClient::send(char *buffer, uint32_t size)
{
	sendMutex.lock();
	data = buffer;
	dataSize = size;
	dataToSend = true;
	sendMutex.unlock();
}

void	UdpClient::sendData()
{
	sendMutex.lock();
	if (dataToSend)
	{
		socklen_t serverAddressLength = sizeof(RoomServerAddress);
		ssize_t bytesSent = sendto(socketFd, data, dataSize, 0,
				(struct sockaddr*)&RoomServerAddress, serverAddressLength);//init handshake
		if (bytesSent == -1)
		{
			std::cerr << color::red << "Failed to init handshake\n" << color::reset;
		}
	}
	sendMutex.unlock();
}