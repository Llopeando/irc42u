#include "../include/ServerData.hpp"
#include "../include/Utils.hpp"

#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <fcntl.h>


START_SERVER_DATA_NAMESPACE

ServerData::ServerData(t_serverInput &serverInfo)
{
	creationDate = utils::t_chrono::to_time_t(utils::t_chrono::now());
	pollfd server;
	Client root(serverInfo.serverName);

	server.events = POLLOUT | POLLIN;
	setSocket(server, serverInfo);

	pollfds.push_back(server);
	clients.push_back(root);
	back.push_back(root);
	channels.push_back(Channel("Lobby", clients[0].getUsername(), this));

	serverName = serverInfo.serverName;
	password = serverInfo.password;

	std::vector<std::string> file = utils::split(utils::readFile("operBlock.txt"), '\n');
	for (std::vector<std::string>::iterator i = file.begin(); i != file.end(); i++)
	{
		std::vector<std::string>line = utils::split(*i, '@');
		if (line.size() == 2)
			operblock[line[0]] = line[1];
	}
}

ServerData::~ServerData()
{
	
}

uint32_t	ServerData::getNumOfChannels() const{

	return (channels.size());
}

uint32_t	ServerData::getNumOfClients() const{

	return (pollfds.size());
}


std::string ServerData::getName() const
{
	return(serverName);
}

char	*ServerData::getCreationDate()const
{
	return std::ctime(&creationDate);
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

//--------------------------------------/ OPERATOR BLOCK /-----------------------------------//


std::string ServerData::getOperList()
{
	std::string list;
	for (std::unordered_map<std::string, std::string>::iterator i = operblock.begin(); i != operblock.end(); i++)
	{
		list += i->first;
		list += ", "; //si es el ultimo ????
	}
	return (list);
}

bool ServerData::checkOperPass(const std::string &user, const std::string& pass) const
{
	std::unordered_map<std::string, std::string>::const_iterator it = operblock.find(user);
	if (it != operblock.end())
		return it->second == pass;
	
	return false;
}

bool ServerData::findOper(const std::string &user)const
{
	return  operblock.find(user) != operblock.end();
}

//--------------------------------------/ CHANNEL OPERATIONS /-----------------------------------//


void ServerData::removeClientChannels(clientIt index)
{
	for(std::deque<Channel>::iterator channel = channels.begin(); channel != channels.end(); channel++)
	{
		if(channel->findUser(index))
		{
			channel->removeClient(index);
		}
	}
}

 void	ServerData::deleteChannel(uint32_t channel)
{
	channels.erase(channels.begin() + channel);
}


uint32_t	ServerData::findChannel(const std::string &name) const
{
	for (uint32_t i =0;i < channels.size();i++)
	{
		if (channels[i].getName() == name)
			return i;
	}
	return (0);
}

void	ServerData::addChannel(std::string name, std::string username, ServerData &serverData)
{
	channels.push_back(Channel(name, username, &serverData));
}


//------------------------------------------------------------------------------------------------//

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