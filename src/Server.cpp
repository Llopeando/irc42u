#include <chrono>
#include "../include/Server.hpp"
#include "../include/commands.h"
#include "../include/cmd_structs.h"
#include "../include/function.h"
#include "../include/ServerDataStructs.h"

/* ------------------------------------------------------------ */
/* 			CONSTRUCTOR DESTRUCTOR INITIALIZATION 				*/
/* ------------------------------------------------------------ */

Server::Server(sd::t_serverInput &serverInput)
			:serverInfo(serverInput),serverData(serverInfo){
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
		//printf("-------- INCOMING REQUEST RECEIVED --------\n");
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
		perror("listening process failure");
		exit(EXIT_FAILURE);
	}
}

#include <arpa/inet.h>

std::string getHostName(int fd)
{
	struct sockaddr_in clientAddr;
	socklen_t clientAddrLen = sizeof(clientAddr);
	if (getpeername(fd, (struct sockaddr *)&clientAddr, &clientAddrLen) == -1) {
		perror("getpeername error");
		exit(1);
	}

	// Convert the IP address from binary to string representation
	char ipStr[INET_ADDRSTRLEN];
	if (inet_ntop(AF_INET, &(clientAddr.sin_addr), ipStr, INET_ADDRSTRLEN) == nullptr) {
		perror("inet_ntop error");
		exit(1);
	}
	return std::string(ipStr);

	/*
	struct addrinfo {
	               int              ai_flags;
	               int              ai_family;
	               int              ai_socktype;
	               int              ai_protocol;
	               socklen_t        ai_addrlen;
	               struct sockaddr *ai_addr;
	               char            *ai_canonname;
	               struct addrinfo *ai_next;
	           };
	
	struct sockaddr {
	        ushort  sa_family;
	        char    sa_data[14];
	};
	*/

}

void	Server::acceptConnection() {
	
	unsigned int size = static_cast<unsigned int>(sizeof(serverInfo.address));
	struct pollfd new_client;

	std::cout << "The connection has been accepted, continuing" << std::endl;
	if ((new_client.fd = accept(serverData[(sd::pollfdIt)0].fd, (struct sockaddr *)&serverInfo.address, &size)) == SERVER_FAILURE)
	{
		perror("connection refused");
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
			else if (conf[0] == "CHANLIMIT")
				serverInput->modes = std::stoi(conf[1].substr(2));
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

void Server::handleInput(sd::clientIt index, std::string input) 
{
	
	/////////////SIGHANDLER o ////////////////
	
	std::vector<std::string> arguments = utils::splitIrcPrameters(input, ' ');

	cmd::CmdInput package(arguments, serverData, index);
	if (cmd::callFunction(arguments[0], package) != cmd::eSuccess)
	{
		std::cout << color::red << "[ERROR DE COMANDO [" <<  input << "]\n" << color::reset;
		//send error message client;
	}
}

void Server::handleEvents(sd::pollfdIt index)
{
	if (serverData[index].revents & POLLIN)
	{
		std::string input = readTCPInput(serverData[index].fd);
		serverData[(sd::clientIt)index].addBuffer(input);
		if (input[input.size() - 1] != '\n')
			return;
		else
		{
			input = serverData[(sd::clientIt)index].getBuffer();
			std::vector<std::string> lines = utils::split(input, '\n');
			for (uint32_t i = 0; i < lines.size(); i++)
			{
				std::cout << color::boldyellow << serverData[(sd::clientIt)index].getUsername() << color::green << " >> CLIENT:[" << color::boldwhite << lines[i] << color::green << "]" << color::reset << "\n" ;
				handleInput((sd::clientIt)index, lines[i]);
			}
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



std::string Server::readTCPInput(int client_fd) {
	char echoBuffer[RCVBUFSIZE];
	int	recvMsgSize;

	memset(echoBuffer, 0, RCVBUFSIZE);
	recvMsgSize = recv(client_fd, echoBuffer, sizeof(echoBuffer) - 1, 0);
	if (recvMsgSize == SERVER_FAILURE)
	{
		perror("recv failed, debug here: ");
		return (std::string(nullptr));
	}
	else if (recvMsgSize == 0) {
		std::vector<std::string> quit_arguments;

		//QUIIIIIIIIIIIIIIIIIIIIIIIITTTTTTTT

		//errorHandler (severfailure)

		//cmd::CmdInput input(quit_arguments, serverData, );  
		//cmd::callfunction("QUIT", input);
		//quit(client_fd, quit_arguments);
		
		return std::string("A client was disconnected from the server");
	}
	///////////////LIMPIACARRO///////////
	ssize_t index = 0;
	for(ssize_t i = 0; i < recvMsgSize; i++)
	{
		if (echoBuffer[i] != '\r')
		{
			echoBuffer[index] = echoBuffer[i];
			index++;
		}
	}
	write(0, echoBuffer, index);
	return std::string(echoBuffer, recvMsgSize);
}

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

void *Server::lauchWrapper(void *data)
{
	Server *server = reinterpret_cast<Server *>(data);
	server->lauch();
	return nullptr;
}

void	Server::run2(){
	

	if(pthread_create(&serverThread, NULL, lauchWrapper, this))
	{
		std::cerr << "Error creating thread" << std::endl;
		return ;
	}
	minishell();
	if(pthread_join(serverThread, NULL))
	{
		std::cerr << "Error joining thread" << std::endl;
		return ;
	}
}

void Server::minishell()
{
	while (1)
	{
		std::string line;
		std::cout << "\t> ";
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
	}
}

void Server::printAllChannNames() const
{
	for (sd::channIt ch = 0; ch < serverData.getNumOfChannels(); ch++)
	{
		std::cout << color::boldwhite << "\t[" << std::to_string(ch) << "]\t" << serverData[ch].getName() << "\n" << color::reset;
	}
}

void Server::printAllUsers()const
{
	for (sd::clientIt user = 0; user < serverData.getNumOfClients(); user++)
	{
		std::cout << color::boldwhite << "\t[" << std::to_string(user) << "] " << serverData[user].getNickname() << color::reset << '\n';
	}
}

void Server::printChannelInfo(const std::string& chName)const
{
	sd::channIt channel = serverData.findChannel(chName);
	if (channel == 0)
	{
		std::cout << color::red << "Channel " << chName << " does not exist\n" << color::reset;
		return ;
	}
	std::cout << color::boldwhite << "\tName: " << color::reset << chName << '\n';
	std::cout << color::boldwhite << "\tTopic: " << color::reset << serverData[channel].getTopic() << '\n';
	std::cout << color::boldwhite << "\tCreator: " << color::reset << serverData[channel].getCreator() << '\n';
	std::cout << color::boldwhite << "\tCreation Date: " << color::reset << serverData[channel].getCreationDate() << '\n';
	std::cout << color::boldwhite << "\tNum of users: " << color::reset << serverData[channel].getNumUser() << '\n';
	std::cout << color::boldwhite << "\tUsers:\n" << color::reset;
	std::vector<std::string> names = utils::split(serverData[channel].getUserList(), '\n');
	for (std::vector<std::string>::const_iterator name = names.begin(); name != names.end(); name++)
	{
		std::cout << color::cyan << "\t\t" << *name << '\n' << color::reset;
	}
}