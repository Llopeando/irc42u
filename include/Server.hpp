/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ullorent <ullorent@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 16:20:01 by ecamara           #+#    #+#             */
/*   Updated: 2023/04/04 19:48:08 by ullorent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#define RCVBUFSIZE 32

#include <iostream>
#include <vector>
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
#include "irc.h"
#include "Channel.hpp"
#include "Client.hpp"


class Server
{
	public:
		Server();
		~Server();

		void	run();
		void	setSocket(t_serverInput serverCreateInfo);
		
	private:
		void	newClient();
		void	cleanUp();
		void	acceptConnection();
		void	listenConnection();
		void	checkFds(int events);
		void	iterFds(void (Server::*func)(uint32_t index));
		void	handleEvents(uint32_t index);
		void	createChannel(std::string name);
		void	deleteChannel(size_t channelIndex);
		std::string readTCPInput(int client_fd);
		int		handleNewUser(struct pollfd pollfd);
		int		selectChannel(uint32_t index, struct pollfd pollfd);

		bool	checkServerPassword(Client *client, uint32_t index, std::string input);
		bool	selectUsername(Client *client, uint32_t index, std::string input);
		bool	checkPassword(Client *client, uint32_t index, std::string input);
		void	selectNickname(Client *client, uint32_t index, std::string input);
		bool	joinChannel(uint32_t index, std::string input);
		//bool	writeInChannel(std::string input);

		void	sendMsgUser(int fd, char const *str);
		void	showChannelsUser(int fd);

		void microshell(uint32_t index, std::string &input);

		//funciones de comandos///
		void nickname_edit();
		void password_edit();
		void role_edit();
		void join_channel();
		void leave_channel();
		void leave_server();
		void susurro();

		int	status;
		int	accptConnection;
		t_serverInput serverInfo;
			//struct sockaddr_in address;
			//std::string password;
		std::deque<struct pollfd> pollfds;
		std::deque<activeIndex> actives;
		std::deque<Client> registered;
		std::deque<Client> tempClients;
		std::deque<Channel> channels;
		t_commands commands;
		
};

#endif