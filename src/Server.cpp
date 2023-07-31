/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ullorent <ullorent@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 18:14:04 by ullorent          #+#    #+#             */
/*   Updated: 2023/07/31 13:22:14 by ullorent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <chrono>
#include "../include/Server.hpp"


//#include "../include/commands.h"
//#include "../include/function.h"


/* ------------------------------------------------------------ */
/* 			CONSTRUCTOR DESTRUCTOR INITIALIZATION 				*/
/* ------------------------------------------------------------ */

Server::Server(sd::t_serverInput &serverInput) :
	serverInfo(serverInput),serverData(serverInfo)
{
	
}

Server::~Server()
{
	if ((status = close(serverData[(sd::pollfdIt)0].fd) == SERVER_FAILURE))
	{
		perror("socket closing failed");
		exit(EXIT_FAILURE);
	}
}

/* ------------------------------------------------------------	*/
/*							MAIN								*/
/* ------------------------------------------------------------	*/


void	Server::run(){
	int events;
	
	listenConnection();
	while(1)
	{
		//printServerStatus();
		events = poll(serverData.getPollfdData(), static_cast<nfds_t>(serverData.pollfdSize()), 200);//200 mirar mas tarde
		if (events < 0)
		{
			perror("error-event detected");
			return;
		}
		if (events != 0)
			checkFds(events); 
	}
}

void	Server::checkFds(int events)
{
	sd::pollfdIt i = 0;
	if (serverData[i].revents & POLLIN) //CONEXION REQUEST
	{
		acceptConnection();
		events--;
	}
	for (i = 1; i < serverData.pollfdSize() && events; i++) //INPUT REQUEST
	{
		handleEvents(i);
		events--;
	}
}

/* ------------------------------------------------------------ */
/*				LISTEN AND ACCEPT CONNECTION					*/
/* ------------------------------------------------------------ */

void	Server::listenConnection() {
	std::cout << "Server started, im listening" << std::endl;
	if (listen(serverData[(sd::pollfdIt)0].fd, 3) == SERVER_FAILURE)
	{
		std::cerr << color::red << "ERROR: listening process failure\n" << color::reset; 
		exit(EXIT_FAILURE);
	}
}

#include <arpa/inet.h>

std::string getHostName(int fd)
{
	struct sockaddr_in clientAddr;
	socklen_t clientAddrLen = sizeof(clientAddr);
	if (getpeername(fd, (struct sockaddr *)&clientAddr, &clientAddrLen) == -1) {
		std::cerr << color::red << "ERROR: getpeername() failed\n" << color::reset; 
		exit(1);
	}
	// Convert the IP address from binary to string representation
	char ipStr[INET_ADDRSTRLEN];
	if (inet_ntop(AF_INET, &(clientAddr.sin_addr), ipStr, INET_ADDRSTRLEN) == nullptr) {
		std::cerr << color::red << "ERROR: inet process failure\n" << color::reset; 
		exit(1);
	}
	return std::string(ipStr);
}

void	Server::acceptConnection() {
	
	unsigned int size = static_cast<unsigned int>(sizeof(serverInfo.address));
	struct pollfd new_client;

	if ((new_client.fd = accept(serverData[(sd::pollfdIt)0].fd, (struct sockaddr *)&serverInfo.address, &size)) == SERVER_FAILURE)
	{
		std::cerr << color::red << "ERROR: Connection refused\n" << color::reset; 
		return;
	}
	new_client.events = POLLOUT | POLLIN;
	std::string hostName = getHostName(new_client.fd);
	serverData.addClient(new_client, Client(hostName));
}

void Server::serverConfig(sd::t_serverInput *serverInput)
{
	std::string file = utils::readFile("server.conf");
	std::vector<std::string>params = utils::split(file, '\n');
	try{
		for (std::vector<std::string>::iterator it = params.begin(); it != params.end(); it++)
		{
			std::vector<std::string> conf = utils::split(*it, '=');
			if (conf[0] == "CHANTYPES")
				serverInput->chantypes = conf[1];
			else if (conf[0] == "PREFIX")
				serverInput->prefix = conf[1];
			else if (conf[0] == "MODES")
				serverInput->modes = std::stoi(conf[1]);
			else if (conf[0] == "CHANLIMIT")
				serverInput->chanlimit = std::stoi(conf[1].substr(2));
			else if (conf[0] == "NICKLEN")
				serverInput->nicklen = std::stoi(conf[1]);
			else if (conf[0] == "USERLEN")
				serverInput->userlen = std::stoi(conf[1]);
			else if (conf[0] == "HOSTLEN")
				serverInput->hostlen = std::stoi(conf[1]);
			else if (conf[0] == "TOPICLEN")
				serverInput->topiclen = std::stoi(conf[1]);
			else if (conf[0] == "KICKLEN")
				serverInput->kicklen = std::stoi(conf[1]);
			else if (conf[0] == "CHANNELLEN")
				serverInput->channellen = std::stoi(conf[1]);
			else if (conf[0] == "MAXUSERS")
				serverInput->maxusers = std::stoi(conf[1]);
			else if (conf[0] == "MAXUSERSCHAN")
				serverInput->maxuserschan = std::stoi(conf[1]);
			else if (conf[0] == "VERSION")
				serverInput->version = conf[1];
			else if (conf[0] == "VERSION_COMMENTS")
				serverInput->versionComments = conf[1];
		}
	}
	catch(...)
	{
		throw std::runtime_error("failed to fill server conf");
	}
}

/* ---------------------------------------------------------------------------------------- */
/*						POLL() AND HANDLE EVENTS	 (incoming requests and inputs)	    //La nueva minishell		*/
/* ---------------------------------------------------------------------------------------- */

eFlags Server::handleInput(sd::clientIt index, std::string input)
{
	std::vector<std::string> arguments = utils::splitIrcPrameters(input, ' ');

	cmd::CmdInput package(arguments, serverData, index);
	
	if (serverData[index].getAuthentificied() != sd::eAuthentified &&
		arguments[0] != "NICK" && arguments[0] != "USER" && arguments[0] != "PASS")
		return eError;

	eFlags output = cmd::callFunction(arguments[0], package);
	
	// Recoger eFLAG y gestionar el cliente segun eFlag
	if (output & eBack)
	{
		cmd::removeClientChannels(serverData, index);
		serverData.backClient(index);
	}
	else if (output & eExited)
	{
		cmd::removeClientChannels(serverData, index);
		serverData.removeClient(index);
	}
	else if (output & eRemoveClientChannel) //probablemente falle por devolver el casteo
	{
		cmd::removeClientChannels(serverData, index);
	}
	else if (output == eNoSuchFunction && arguments.size() &&!arguments[0].empty())
	{
		std::cerr << color::red << "ERR_UNKNOWNCOMMAND: [" <<  arguments[0] << "]\n" << color::reset; 
		error::error(package, error::ERR_UNKNOWNCOMMAND, arguments[0]);
	}
	return output;
}

void Server::handleEvents(sd::pollfdIt index)
{
	if (serverData[index].revents & POLLIN)
	{
		std::string input = readTCPInput(serverData[index].fd, (sd::clientIt)index);
		std::cout << color::blue << "input: [" << input << "]\n" << color::reset;
		if ("QUIT" == input)
			return;
		serverData[(sd::clientIt)index].addBuffer(input);
		if (input[input.size() - 1] != '\n')
			return;
		else
		{
			input = serverData[(sd::clientIt)index].getBuffer();
			std::vector<std::string> lines = utils::split(input, '\n');
			bool exited = false;
			for (uint32_t i = 0; i < lines.size(); i++)
			{
				if (handleInput((sd::clientIt)index, lines[i]) == eExited)
					exited = true;
			}
			if (!exited)
				serverData[(sd::clientIt)index].emptyBuffer();
		}
	}
}


std::string Server::getName()const
{
	return serverData.getName();
}


/* ------------------------------------------------------------ */
/*							UTILS								*/
/* ------------------------------------------------------------ */



std::string Server::readTCPInput(int client_fd, sd::clientIt index) {
	char echoBuffer[RCVBUFSIZE];
	int	recvMsgSize;
	

	memset(echoBuffer, 0, RCVBUFSIZE);
	recvMsgSize = recv(client_fd, echoBuffer, sizeof(echoBuffer) - 1, 0);
	if (recvMsgSize == SERVER_FAILURE)
	{
		std::cerr << color::red << "ERROR: recv failed\n" << color::reset; 
		return (std::string(nullptr));
	}
	else if (recvMsgSize == 0) {
		std::vector<std::string>arguments;
		arguments.push_back("QUIT");
		cmd::CmdInput package(arguments, serverData, index);
		cmd::callFunction("QUIT", package);
		return("QUIT");
	}
	///////////////LIMPIACARRO///////////
	ssize_t j = 0;
	for(ssize_t i = 0; i < recvMsgSize; i++)
	{
		if (echoBuffer[i] != '\r')
		{
			echoBuffer[j] = echoBuffer[i];
			j++;
		}
	}
	return std::string(echoBuffer, j);
}

//
//
//
//
//void Server::removeClientChannels(sd::clientIt index)
//{
//	uint32_t i = 1;
//	std::vector<uint32_t> deleteChannels;
//	for(std::deque<sd::Channel>::iterator channel = serverData.getChannelBegin() + 1; channel != serverData.getChannelEnd(); channel++)
//	{
//		if(channel->findUser(index))
//		{
//			if (channel->removeClient(index) == eRemoveChannel)
//				deleteChannels.push_back(i);
//		}
//		i++;
//	}
//	i = 0;
//	for (std::vector<uint32_t>::iterator deleteChannel = deleteChannels.begin();
//			deleteChannel != deleteChannels.end(); deleteChannel++)
//	{
//		std::string creatorName = serverData[(sd::channIt)(*deleteChannel - i)].getCreator();
//		sd::clientIt creator = serverData.findUsername(creatorName);
//		if (creator == 0)
//		{
//			creator = serverData.findUsernameBack(creatorName);
//			if (creator == 0)
//				continue ;
//			serverData[(sd::backIt)creator].updateOps(*deleteChannel - i);
//		}
//		else
//			serverData[creator].updateOps(*deleteChannel - i);
//		//input.serverData[channel].broadcast(0, reply(eRPL_PART, input));
//
//		/* MOVE TO SERVER */
//		std::vector<std::string> arguments;
//		arguments.push_back("PART");
//		arguments.push_back(serverData[(sd::channIt)(*deleteChannel - i)].getName());
//		cmd::CmdInput input(arguments, serverData, index);
//		
//		cmd::callFunction("PART", input);
//		serverData.getChannel().erase(serverData.getChannelBegin() + *deleteChannel - i);
//		i++;
//	}
//}
//

/* ---------------------------------------------------------------------------------------- */
/*										DEBUG PRINT											*/
/* ---------------------------------------------------------------------------------------- */


void	Server::printServerStatus() const
{
	static std::chrono::steady_clock::time_point last_time = std::chrono::high_resolution_clock::now();
	std::chrono::steady_clock::time_point now;
	double timeElapsed;

	now = std::chrono::high_resolution_clock::now();
	timeElapsed = std::chrono::duration_cast<std::chrono::duration<double> >(now - last_time).count();
	if (timeElapsed > 1)
	{
		system("clear");
		utils::printIp();
		std::cout << "≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡ 🖥️  SERVER STATUS ℹ️  ≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡\n";
	
		//std::cout << "🖥️  👥 SERVER ACTIVE USERS: " << data.size() << '\n' << std::endl;

		std::cout << "USERS (waiting to login or register): " << serverData.pollfdSize() << '\n';
		for(sd::clientIt i = 0; i < serverData.pollfdSize(); i++)
		{
			std::cout << "\t[" << i << "]  username: " << serverData[i].getUsername() << " nickname: " << serverData[i].getNickname() << " role: " << serverData[i].getRole() << "\n";
		}

		//std::cout << "\t\t CHANNELS LIST:" << channels.size() << "\n";
		//for(uint32_t i = 0; i < channels.size(); i++)
		//{
		//	std::cout << "[" << channels[i].getName() << "]" << std::endl;
		//}
		std::cout << "DEBUG LOG (if any)" << std::endl;
		last_time = now;
	}
}

/* ---------------------------------------------------------------------------------------- */
/*										SERVER FROM ITERM									*/
/* ---------------------------------------------------------------------------------------- */

void Server::lauch()
{
	int events;

	listenConnection();
	while(1)
	{
		//printServerStatus();
		events = poll(serverData.getPollfdData(), static_cast<nfds_t>(serverData.pollfdSize()), 200);//200 mirar mas tarde MAXUSERS
		if (events < 0)
		{
			std::cerr << color::red << "ERROR: poll() - error-event detected\n" << color::reset; 
			return;
		}
		if (events != 0)
			checkFds(events);
	}
}

void *Server::lauchWrapper(void *data)
{
	Server *server = reinterpret_cast<Server *>(data);
	server->lauch();
	return nullptr;
}

void	Server::run2()
{
	if(pthread_create(&serverThread, NULL, lauchWrapper, this))
	{
		std::cerr << color::red << "ERROR: Error creating thread\n" << color::reset; 
		return ;
	}
	minishell();
	if(pthread_join(serverThread, NULL))
	{
		std::cerr << color::red << "ERROR: Error joining thread\n" << color::reset; 
		return ;
	}
}

void Server::minishell()
{
	std::cout << std::endl;
	while (1)
	{
		std::string line;
		std::cout << color::boldwhite << "> " << color::reset;
		std::getline(std::cin, line);
		if (line == "quit")
			return ;
		else if (line == "ls ch")
			printAllChannNames();
		else if (line == "ls users")
			printAllUsers();
		else if (line == "info ch")
		{
			std::cout << "ch name: ";
			std::getline(std::cin, line);
			printChannelInfo(line);
		}
		else if (line == "info user")
		{
			std::cout << "user name: ";
			std::getline(std::cin, line);
			printUserInfo(line);
		}
		else if (line == "info server")
			printServerInfo();
		else if (line == "info")
			printInfo();
		else if (line == "rm -rf")
		{
			std::cout << "user nickname: ";
			std::getline(std::cin, line);
			rmrf(line);
		}
		else {
			std::cout << color::red << "║ Unknown command!\n" << color::reset;
		}
	}
}

void Server::printAllChannNames() const
{
	for (sd::channIt ch = 0; ch < serverData.getNumOfChannels(); ch++)
	{
		std::cout << color::boldwhite << "║ [" << std::to_string(ch) << "]\t" << serverData[ch].getName() << "\n" << color::reset;
	}
}

void Server::printAllUsers()const
{
	for (sd::clientIt user = 0; user < serverData.getNumOfClients(); user++)
	{
		std::cout << color::boldwhite << "║ [" << std::to_string(user) << "] " << serverData[user].getNickname() << color::reset << '\n';
	}
}

void Server::printChannelInfo(const std::string& chName)const
{
	sd::channIt channel = serverData.findChannel(chName);
	if (channel == 0)
	{
		std::cout << color::red << "║ Channel '" << chName << "' does not exist\n" << color::reset;
		return ;
	}
	std::cout << color::boldwhite << "║ " << color::yellow << "Name: " << color::reset << chName << '\n';
	std::cout << color::boldwhite << "║ " << color::yellow << "Topic: " << color::reset << serverData[channel].getTopic() << '\n';
	std::cout << color::boldwhite << "║ " << color::yellow << "Creator: " << color::reset << serverData[channel].getCreator() << '\n';
	std::cout << color::boldwhite << "║ " << color::yellow << "Creation Date: " << color::reset << serverData[channel].getCreationDate() << '\n';
	std::cout << color::boldwhite << "║ " << color::yellow << "Num of users: " << color::reset << serverData[channel].getNumUser() << '\n';
	std::cout << color::boldwhite << "║ " << color::yellow << "Users:\n" << color::reset;
	std::vector<std::string> names = utils::split(serverData.getUserList(channel), '\n');
	for (std::vector<std::string>::const_iterator name = names.begin(); name != names.end(); name++)
	{
		std::cout << color::cyan << "\t" << *name << '\n' << color::reset;
	}
}


void Server::printUserInfo(const std::string& nickname)const
{
	sd::clientIt user = serverData.findNickname(nickname);
	if (user == 0)
	{
		std::cout << color::red << "║ Channel " << nickname << " does not exist\n" << color::reset;
		return ;
	}
	std::cout << color::boldwhite << "║ " << color::yellow << "Nickname: " << color::reset << nickname << '\n';
	std::cout << color::boldwhite << "║ " << color::yellow << "Username: " << color::reset << serverData[user].getUsername() << '\n';
	std::cout << color::boldwhite << "║ " << color::yellow << "Hostname: " << color::reset << serverData[user].getHostname() << '\n';
	std::cout << color::boldwhite << "║ " << color::yellow << "Role: " << color::reset << serverData[user].getRole() << '\n';
	std::cout << color::boldwhite << "║ " << color::yellow << "Authentified: " << color::reset << serverData[user].getAuthentificied() << '\n';
		std::cout << color::boldwhite << "║ " << color::yellow << "ChannelOps: " << color::reset;
		utils::printBinary(serverData[user].getChannelToOps());
		std::cout << '\n';
	std::cout << color::boldwhite << "║ " << color::yellow << "Away: " << color::reset << (bool)serverData[user].getAwayStatus();
	if (serverData[user].getAwayStatus())
		std::cout << color::boldwhite << "║ Message: " << color::reset << serverData[user].getAwayMsg();
	std::cout << '\n';
}

void Server::rmrf(const std::string& nickname)
{
	sd::clientIt user = serverData.findNickname(nickname);
	if (user == 0)
	{
		std::cout << color::red << "║ Channel " << nickname << " does not exist\n" << color::reset;
		return ;
	}
	serverData.removeClient(user);
}

void Server::printServerInfo()const
{
	std::cout << color::boldwhite << "║ " << color::yellow << "CHANTYPES: " << color::reset << serverInfo.chantypes << '\n';
	std::cout << color::boldwhite << "║ " << color::yellow << "PREFIX: " << color::reset << serverInfo.prefix << '\n';
	std::cout << color::boldwhite << "║ " << color::yellow << "MODES: " << color::reset << serverInfo.modes << '\n';
	std::cout << color::boldwhite << "║ " << color::yellow << "CHANLIMIT: " << color::reset << serverInfo.chanlimit << '\n';
	std::cout << color::boldwhite << "║ " << color::yellow << "NICKLEN: " << color::reset << serverInfo.nicklen << '\n';
	std::cout << color::boldwhite << "║ " << color::yellow << "USERLEN: " << color::reset << serverInfo.userlen << '\n';

	std::cout << color::boldwhite << "║ " << color::yellow << "HOSTLEN: " << color::reset << serverInfo.hostlen << '\n';
	std::cout << color::boldwhite << "║ " << color::yellow << "TOPICLEN: " << color::reset << serverInfo.topiclen << '\n';
	std::cout << color::boldwhite << "║ " << color::yellow << "CHANNELLEN: " << color::reset << serverInfo.channellen << '\n';
	std::cout << color::boldwhite << "║ " << color::yellow << "MAXUSERS: " << color::reset << serverInfo.maxusers << '\n';
	std::cout << color::boldwhite << "║ " << color::yellow << "MAXUSERSCHAN: " << color::reset << serverInfo.maxuserschan << '\n';
	std::cout << color::boldwhite << "║ " << color::yellow << "VERSION: " << color::reset << serverInfo.version << '\n';
	std::cout << color::boldwhite << "║ " << color::yellow << "USERLEN: " << color::reset << serverInfo.userlen << '\n';
	std::cout << color::boldwhite << "║ " << color::yellow << "VERSION_COMMENTS: " << color::reset << serverInfo.versionComments << '\n';
}

void Server::printInfo()const
{
	std::cout << color::boldwhite << "║ " << color::yellow << "ls ch: " << color::reset << "list all channels" << '\n';
	std::cout << color::boldwhite << "║ " << color::yellow << "ls users: " << color::reset << "list all users" << '\n';
	std::cout << color::boldwhite << "║ " << color::yellow << "info user: " << color::reset << "print user info" << '\n';
	std::cout << color::boldwhite << "║ " << color::yellow << "info ch: " << color::reset << "print channel info" << '\n';
	std::cout << color::boldwhite << "║ " << color::yellow << "info server: " << color::reset << "print server info" << '\n';
}