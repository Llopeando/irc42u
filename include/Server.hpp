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
#include <pthread.h>


#include "Utils.hpp"
#include "defines.hpp"
#include "color.h"
#include "ServerData.hpp"
#include "cmd_structs.h"

class Server
{
	public:
		Server(sd::t_serverInput &serverInput);
		~Server();

		void	run();
		void	printServerStatus() const;
		std::string	getName()const;
		static void serverConfig(sd::t_serverInput *serverInput);

		void	run2();
	private:

		int	status;
		sd::t_serverInput serverInfo;
		sd::ServerData serverData;
		pthread_t serverThread;

		void		acceptConnection();
		void		listenConnection();
		void		handleNewUser(std::string &input, sd::clientIt);
		cmd::eFlags	handleInput(sd::clientIt index, std::string input);
		void		sendCapabilities(sd::pollfdIt index);
		void		acknowledgeCapabilities(sd::pollfdIt index, std::string input);

		void		saveNick(std::vector<std::string> &arguments, Client &newUser);
		void		saveUser(std::vector<std::string> &arguments, Client &newUser);

		void		checkFds(int events);
		void		handleEvents(sd::pollfdIt index);

		std::string	readTCPInput(int client_fd, sd::clientIt index);
		void		setCommands();

		static void *lauchWrapper(void *data);
		void lauch();
		void minishell();
		void	printAllChannNames() const;
		void	printAllUsers() const ;
		void	printChannelInfo(const std::string& chName) const;
		void	printUserInfo(const std::string& nickname) const;
		void	printServerInfo()const;
		void	printInfo() const;
};

#endif