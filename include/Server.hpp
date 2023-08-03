/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ullorent <ullorent@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 17:43:28 by ullorent          #+#    #+#             */
/*   Updated: 2023/08/03 20:38:17 by ullorent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

		void	acceptConnection();
		void	listenConnection();
		void	handleNewUser(std::string &input, clientIt);
		void	handleInput(clientIt index, std::string input);
		void	sendCapabilities(pollfdIt index);
		void	acknowledgeCapabilities(pollfdIt index, std::string input);

		void	saveNick(std::vector<std::string> &arguments, Client &newUser);
		void	saveUser(std::vector<std::string> &arguments, Client &newUser);

		void	checkFds(int events);
		void	handleEvents(pollfdIt index);
		std::string	readTCPInput(int client_fd);
		//void	sendMsgUser(int fd, const std::string &str) const;
		//void	sendMsgUser(clientIt it, const std::string &str) const;
		void	setCommands();

		void	deleteChannel(uint32_t channel);
		void 	removeClientChannels(clientIt index);
		uint32_t	findChannel(const std::string &name) const;

		//COMMAND MESSAGES

			//Connection messages//

		void	cap(clientIt index, std::vector<std::string> &arguments);
				// CAP FUNCTIONS
		void	cap_req(clientIt index, std::vector<std::string> &arguments);
		void	cap_ls(clientIt index, std::vector<std::string> &arguments);
		void	cap_end(clientIt index, std::vector<std::string> &arguments);
		void	cap_ack(clientIt index, std::vector<std::string> &arguments);
		void	cap_nak(clientIt index, std::vector<std::string> &arguments);

		void	pass(clientIt index, std::vector<std::string> &arguments);
		void	nick(clientIt index, std::vector<std::string> &arguments);
		void	user(clientIt index, std::vector<std::string> &arguments);
		void	ping(clientIt index, std::vector<std::string> &arguments);
		void	oper(clientIt index, std::vector<std::string> &arguments);
		void	quit(clientIt index, std::vector<std::string> &arguments);

			//Channel Operations//

		void	join(clientIt index, std::vector<std::string> &arguments);
		void	part(clientIt index, std::vector<std::string> &arguments);
		void	topic(clientIt index, std::vector<std::string> &arguments);
		void	names(clientIt index, std::vector<std::string> &arguments);
		void	list(clientIt index, std::vector<std::string> &arguments);
		void	invite(clientIt index, std::vector<std::string> &arguments);
		void	kick(clientIt index, std::vector<std::string> &arguments);

			//Server Queries and Commands//

		void	motd(clientIt index, std::vector<std::string> &arguments);
		void	mode(clientIt index, std::vector<std::string> &arguments);

			//Sending Messages//

		void	privmsg(clientIt index, std::vector<std::string> &arguments);
		void	notice(clientIt index, std::vector<std::string> &arguments);

			//User-Based Queries//

		void	whois(clientIt index, std::vector<std::string> &arguments);
		void	who(clientIt index, std::vector<std::string> &arguments);

			//Operator Messages//

		void	kill(clientIt index, std::vector<std::string> &arguments);

			//Optional Messages//

		void	away(clientIt index, std::vector<std::string> &arguments);	
	

	

		int	status;
		sd::t_serverInput serverInfo;
		sd::ServerData serverData;
		pthread_t serverThread;

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

		static void *lauchWrapper(void *data);
		void	lauch();
		void	minishell();
		void	printAllChannNames() const;
		void	printAllUsers() const ;
		void	printAllUsersBack() const ;
		void	printLobbyInfo()const;
		void	printChannelInfo(const std::string& chName) const;
		void	printUserInfo(const std::string& nickname) const;
		void	printServerInfo()const;
		void	printInfo() const;
		void	rmrf(const std::string& nickname);
		void	updateConf();
};

#endif