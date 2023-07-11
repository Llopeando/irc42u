#include <chrono>
#include "../include/Server.hpp"
#include "../include/commands.h"
#include "../include/command_structs.h"
#include "../include/function.h"
/* ------------------------------------------------------------ */
/* 			CONSTRUCTOR DESTRUCTOR INITIALIZATION 				*/
/* ------------------------------------------------------------ */

Server::Server(sd::t_serverInput &serverInput)
			:serverInfo(serverInput),serverData(serverInfo){}

Server::~Server()
{
	if ((status = close(data[(pollfdIt)0].fd) == SERVER_FAILURE))
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
	//////PINGGGGGGGGGGGGGGGGGG//////
	if ((new_client.fd = accept(serverData[(sd::pollfdIt)0].fd, (struct sockaddr *)&serverInfo.address, &size)) == SERVER_FAILURE)
	{
		perror("connection refused");
		return;
	}
	new_client.events = POLLOUT | POLLIN;
	std::string hostName = getHostName(new_client.fd);

	serverData.addClient(new_client, Client(hostName));
	//sendMsgUser(data.size() - 1, "Welcome to - A O I R C - \n");
}

/* ---------------------------------------------------------------------------------------- */
/*						POLL() AND HANDLE EVENTS	 (incoming requests and inputs)	    //La nueva minishell		*/
/* ---------------------------------------------------------------------------------------- */

void Server::handleInput(sd::clientIt index, std::string input) 
{
	std::vector<std::string>arguments = utils::splitIrcPrameters(input, ' ');

	cmd::CmdInput package;
	package.arguments = arguments;
	package.server = this;
	package.it = index;
	if (cmd::callFunction(arguments[0], package) != cmd::eFlags::eSuccess)
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
		std::cout << color::green << "INPUT:[" << color::boldwhite << input << "]\n" << color::reset;
		std::vector<std::string> lines = utils::split(input, '\n');
		for (uint32_t i = 0; i < lines.size(); i++)
		{
		//	std::cout << color::boldgreen << "[" << lines[i] << "]\n" << color::reset;
			handleInput((sd::clientIt)index, lines[i]);
		}
	}
}


std::string Server::getName()const
{
	return serverName;
}

/*void	Server::cap_available(std::vector<std::string> &arguments)
{
		//va a devolver las capabilities disponibles, osea las que coinciden 

	std::vector<std::string> availables;


		

}*/


/* ------------------------------------------------------------ */
/*							UTILS								*/
/* ------------------------------------------------------------ */


uint32_t	Server::findChannel(const std::string &name) const
{
	for (uint32_t i =0;i < channels.size();i++)
	{
		if (channels[i].getName() == name)
			return i;
	}
	return (0);
}

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
		quit(client_fd, quit_arguments);
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
/*
uint32_t	Server::findUsername(const std::string &username) const
{
	for (uint32_t i =0;i < data->sizeReg();i++)
	{
		if ((data->Registered()[i].getUsername()) == username)
		{	
			return i;
		}
	}
	return (0);
}

bool	Server::assertClientPassword(uint32_t indexAct, const std::string &password) const
{
	uint32_t indexReg;

	indexReg = findUsername((*data)[indexAct].getUsername());
	if(indexReg == 0)
	{
		std::cout << "server crash\n";
	}
	else
	{
		if ((*data)[indexAct].checkPassword(password))
		{
			return(true);
			
		}
		else
		{
			return (false);
		}
	}
	return (false);
}
*/

/*
bool checkAdmin(Client *client) {
	if (client->getRole() == CL_ROOT)
		return true;
	else
		return false;
}
*/


/* ---------------------------------------------------------------------------------------- */
/*						INIT SETTINGS													*/
/* ---------------------------------------------------------------------------------------- */



void Server::setCommands()
{
	commands.cap_funcmap["CAP_REQ"]	= &Server::cap_req;
	commands.cap_funcmap["CAP_LS"]	= &Server::cap_ls;
	commands.cap_funcmap["CAP_END"]	= &Server::cap_end;
	commands.cap_funcmap["CAP_ACK"]	= &Server::cap_ack;
	commands.cap_funcmap["CAP_NAK"]	= &Server::cap_nak;
}


/* ---------------------------------------------------------------------------------------- */
/*						DEBUG PRINT													*/
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
		printIp();
		std::cout << "≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡ 🖥️  SERVER STATUS ℹ️  ≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡\n";
	
		//std::cout << "🖥️  👥 SERVER ACTIVE USERS: " << data.size() << '\n' << std::endl;

		std::cout << "USERS (waiting to login or register): " << data.size() << '\n';
		for(clientIt i = 0; i < data.size(); i++)
		{
			std::cout << "\t[" << i << "]  username: " << data[i].getUsername() << " nickname: " << data[i].getNickname() << " role: " << data[i].getRole() << "\n";
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

void	Server::deleteChannel(uint32_t channel)
{
	channels.erase(channels.begin() + channel);
}

void Server::removeClientChannels(clientIt index)
{
	for(std::deque<Channel>::iterator channel = channels.begin(); channel != channels.end(); channel++)
	{
		if(channel->findUser(index))
		{
			channel->removeClient(index);
		}
	}
}