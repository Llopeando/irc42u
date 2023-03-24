/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecamara <ecamara@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 16:20:01 by ecamara           #+#    #+#             */
/*   Updated: 2023/03/24 18:05:24 by ecamara          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#define SERVER_FAILURE -1

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
#include "irc.h"
#include "Channel.hpp"


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
		void	iterFds(void (Server::*func)(pollfd &pollfd));
		void	readClientsInput(pollfd &pollfd);
		void	createChannel(std::string name);
		void	deleteChannel(size_t channelIndex);
		int		handleNewUser(struct pollfd pollfd);

		int	status;
		int	accptConnection;
		t_serverInput serverInfo;
			//struct sockaddr_in address;
			//std::string password;
		std::deque<struct pollfd> pollfds;
		std::deque<uint8_t> userChannel;
		std::deque<Channel> channels;
};

void handleTCPClient(int client_fd);

#endif