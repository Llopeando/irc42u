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

#include "defines.h"
#include "Utils.hpp"
#include "ServerDataStructs.h"

#include "Commands.hpp"


class Server
{
	public:
		Server(sd::t_serverInput &serverInput);
		~Server();

		void	run();
		std::string	getName()const;
		static void serverConfig(sd::t_serverInput *serverInput);

		void	run2();
	private:

		int	status;
		sd::t_serverInput serverInfo;
		sd::ServerData serverData;
		pthread_t serverThread;
		std::fstream logFile;

		void		acceptConnection();
		void		listenConnection();
		void		handleNewUser(std::string &input, sd::clientIt);
		eFlags		handleInput(sd::clientIt index, std::string input);
		void		sendCapabilities(sd::pollfdIt index);
		void		acknowledgeCapabilities(sd::pollfdIt index, std::string input);

		void		saveNick(std::vector<std::string> &arguments, Client &newUser);
		void		saveUser(std::vector<std::string> &arguments, Client &newUser);

		void		checkFds(int events);
		void		handleEvents(sd::pollfdIt index);

		std::string	readTCPInput(int client_fd, sd::clientIt index);
		void		setCommands();


		/*LOGGING*/
		void	log(const std::string str);
		void	logServerStatus();
		void	logTimeVal(const std::string str);

		static void *lauchWrapper(void *data);
		void	lauch();
		void	minishell();
		void	printAllChannNames(bool color) const;
		void	printAllUsers(bool color) const ;
		void	printAllUsersBack(bool color) const ;
		void	printLobbyInfo(bool color)const;
		void	printChannelInfo(const std::string& chName) const;
		void	printUserInfo(const std::string& nickname) const;
		void	printServerInfo()const;
		void	printInfo() const;
		void	rmrf(const std::string& nickname);
		void	updateConf();
};

#endif