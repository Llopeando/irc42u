
#include "../include/UsersData.hpp"

UsersData::UsersData()
{
}

UsersData::UsersData(t_serverInput serverInfo)
{
	// posicion 0 de todos los array como INTOCABLES: pollfds 0 SERVER y cliente 0 ROOT
	pollfd server;
	Client root(serverInfo.IP);

	setSocket(server, serverInfo);

	server.events = POLLOUT | POLLIN;
	pollfds.push_back(server);
	clients.push_back(root);
	back.push_back(root);
}

UsersData::~UsersData()
{
	
}

void	UsersData::addClient(pollfd userPollfd,Client newClient)
{
	pollfds.push_back(userPollfd);
	clients.push_back(newClient); //CLIENTE VACIO
}

void	UsersData::setSocket(pollfd &server, t_serverInput &serverInfo){
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

void	UsersData::backClient(clientIt index)
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


void	UsersData::removeClient(clientIt index)
{
	if (index != clients.size() - 1)
	{
		clients[index] = clients[clients.size() - 1];
		pollfds[index] = pollfds[pollfds.size() - 1];
	}
	clients.resize(clients.size() - 1);
	pollfds.resize(pollfds.size() - 1);
}


void	UsersData::forwardClient(const std::string& nickname) //no existe el caso porque no puedes entrar por back 
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

void	UsersData::transferIndex(clientIt index, const std::string& nickname)
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

pollfd *UsersData::getPollfdData()
{
	return pollfds.data();
}

uint32_t UsersData::size()const
{
	return pollfds.size();
}


Client &UsersData::operator[](clientIt idx)
{
	return clients[idx];
}

const Client &UsersData::operator[](clientIt idx) const
{
	return clients[idx];
}

pollfd &UsersData::operator[](pollfdIt idx)
{
	return pollfds[idx];
}

const pollfd &UsersData::operator[](pollfdIt idx) const
{
	return pollfds[idx];
}

clientIt UsersData::findUsername(std::string argument) const
{
	for(clientIt i = 0  ; i < this->size(); i++)
		if (clients[i].getUsername() == argument )
			return(i);
	return (0);
}

clientIt UsersData::findNickname(std::string argument) const
{
	for(clientIt i = 0  ; i < this->size(); i++)
		if (clients[i].getNickname() == argument )
			return(i);
	return (0);
}

clientIt UsersData::findNicknameBack(std::string argument) const
 {
	for(clientIt i = 0  ; i < back.size(); i++)
		if (back[i].getNickname() == argument)
			return(i);
	return (0);
}


