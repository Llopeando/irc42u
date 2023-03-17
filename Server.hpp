/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ullorent <ullorent@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 16:20:01 by ecamara           #+#    #+#             */
/*   Updated: 2023/03/17 19:50:53 by ullorent         ###   ########.fr       */
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
#include "irc.h"


class Server
{
	public:
		Server();
		~Server();

		void	run();
		void	setSocket(t_serverInput serverCreateInfo);
		void	acceptConnection();
		void	listenConnection();
		void	checkFds(int events);
		void	iterFds(pollfd *add, size_t length);
		void	writeFds(pollfd &pollfd);
	private:
		void	newClient();
		void	cleanUp();

		int	status;
		int	accptConnection;
		t_serverInput serverInfo;
			//struct sockaddr_in address;
			//std::string password;
		std::vector<struct pollfd> pollfds;
		
};

void handleTCPClient(int client_fd);

#endif