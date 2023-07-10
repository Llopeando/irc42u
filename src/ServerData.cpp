#include "../include/ServerData.hpp"

#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <fcntl.h>


START_SERVER_DATA_NAMESPACE

ServerData::ServerData(t_serverInput &serverInfo)
{
	pollfd server;
	Client root(serverInfo.serverName);

	server.events = POLLOUT | POLLIN;
	setSocket(server, serverInfo);

	pollfds.push_back(server);
	clients.push_back(root);
	back.push_back(root);
	channels.push_back(Channel("Lobby", clients[0].getUsername(), this));

	serverName = serverInfo.serverName;
}

ServerData::~ServerData()
{
	
}

void	ServerData::addClient(pollfd userPollfd,Client newClient)
{
	pollfds.push_back(userPollfd);
	clients.push_back(newClient); //CLIENTE VACIO
}

void	ServerData::setSocket(pollfd &server, t_serverInput &serverInfo){

	if ((server.fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == SERVER_FAILURE)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	if (bind(server.fd, (const struct sockaddr*)&serverInfo.address, sizeof(serverInfo.address)) == SERVER_FAILURE)
	{
		perror("socket bind failed");
		exit(EXIT_FAILURE);
	}
	
	fcntl(server.fd, F_SETFL, O_NONBLOCK);

}

void	ServerData::backClient(clientIt index)
{
	back.push_back(Client(clients[index]));
	if (index != clients.size() - 1)
	{
		clients[index] = clients[clients.size() - 1];
		pollfds[index] = pollfds[pollfds.size() - 1];
	}
	clients.resize(clients.size() - 1);
	pollfds.resize(pollfds.size() - 1);
}


void	ServerData::removeClient(clientIt index)
{
	if (index != clients.size() - 1)
	{
		clients[index] = clients[clients.size() - 1];
		pollfds[index] = pollfds[pollfds.size() - 1];
	}
	clients.resize(clients.size() - 1);
	pollfds.resize(pollfds.size() - 1);
}


void	ServerData::forwardClient(const std::string& nickname) //no existe el caso porque no puedes entrar por back 
{
	uint32_t index = findNicknameBack(nickname);
	if (index == 0)
		return ;
	clients.push_back(Client(clients[index]));
	if (index != back.size() - 1)
	{
		back[index] = back[clients.size() - 1];
	}
	back.resize(back.size() - 1);
}

void	ServerData::transferIndex(clientIt index, const std::string& nickname)
{
	uint32_t backIndex = findNicknameBack(nickname);
	if (index == 0)
		return ;
	clients[index] = back[backIndex];
	if (backIndex != back.size() - 1)
	{
		back[index] = back[clients.size() - 1];
	}
	back.resize(back.size() - 1);
}

pollfd *ServerData::getPollfdData()
{
	return pollfds.data();
}

clientIt ServerData::findUsername(const std::string& argument) const
{
	for(clientIt i = 0  ; i < clients.size(); i++)
		if (clients[i].getUsername() == argument )
			return(i);
	return (0);
}

clientIt ServerData::findNickname(const std::string& argument) const
{
	for(clientIt i = 0  ; i < clients.size(); i++)
		if (clients[i].getNickname() == argument )
			return(i);
	return (0);
}

clientIt ServerData::findNicknameBack(const std::string& argument) const
 {
	for(clientIt i = 0  ; i < back.size(); i++)
		if (back[i].getNickname() == argument)
			return(i);
	return (0);
}

END_SERVER_DATA_NAMESPACE