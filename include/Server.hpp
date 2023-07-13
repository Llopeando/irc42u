#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <cstdio>
#include <unistd.h>
#include <poll.h>
#include <deque>
#include <fcntl.h>
#include <vector>
#include <set>

#include <cerrno>
#include <system_error>

#include "Utils.hpp"
#include "defines.hpp"
#include "color.h"
#include "ServerData.hpp"

//___________SERVER DEFAULT CONFIG & RPL_ISUPPORT parameters___________
//
//CHANTYPES=# 
//PREFIX=(o)@ 
//MODES=4 
//CHANLIMIT=#:20 
//NICKLEN=16 
//USERLEN=10 
//HOSTLEN=63 
//TOPICLEN=390 
//KICKLEN=307 
//CHANNELLEN=32

//VERSION="1.0"
//VERSION_COMMENTS= "No comments"
//____________________________________________

class Server
{
	public:
		Server(sd::t_serverInput &serverInput);
		~Server();

		void	run();
		void	printServerStatus() const;
		std::string	getName()const;
		
	private:

		int	status;
		sd::t_serverInput serverInfo;
		sd::ServerData serverData;

		void	acceptConnection();
		void	listenConnection();
		void	handleNewUser(std::string &input, sd::clientIt);
		void	handleInput(sd::clientIt index, std::string input);
		void	sendCapabilities(sd::pollfdIt index);
		void	acknowledgeCapabilities(sd::pollfdIt index, std::string input);

		void	saveNick(std::vector<std::string> &arguments, Client &newUser);
		void	saveUser(std::vector<std::string> &arguments, Client &newUser);

		void	checkFds(int events);
		void	handleEvents(sd::pollfdIt index);

		std::string	readTCPInput(int client_fd);
		void	setCommands();


};

#endif