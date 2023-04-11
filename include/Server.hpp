/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecamara <ecamara@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 16:20:01 by ecamara           #+#    #+#             */
/*   Updated: 2023/04/05 11:43:17 by ecamara          ###   ########.fr       */
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

		void	acceptConnection();
		void	listenConnection();

		void	handleEvents(uint32_t index);
		
		bool	checkServerPassword(Client *client, uint32_t index, std::string &input);
		bool	selectUsername(Client *client, uint32_t index, std::string &input);
		bool	checkPassword(Client *client, uint32_t index, std::string &input);
		void	selectNickname(Client *client, uint32_t index, std::string &input);

		void	microshell(uint32_t index, std::string &input);
		void	nickname_edit(uint32_t index, std::string &argument);
		void	password_edit(uint32_t index, std::string &argument);
		void	role_edit(uint32_t index, std::string &argument);
		void	join_channel(uint32_t index, std::string &argument);
		void	leave_channel(uint32_t index, std::string &argument);
		void	leave_server(uint32_t index, std::string &argument);
		void	susurro(uint32_t index, std::string &argument);
		void	createChannel(std::string name);
		void	deleteChannel(size_t channelIndex);

		void		checkFds(int events);
		void		iterFds(void (Server::*func)(uint32_t index));
		bool		assertClientPassword(const std::string &username, const std::string &password)const;
		uint32_t	findUsername(const std::string &username)const;
		std::string	readTCPInput(int client_fd);

		bool	joinChannel(uint32_t index, std::string input);
		//bool	writeInChannel(std::string input);
		void	newClient();
		void	cleanUp();
		void	sendMsgUser(int fd, char const *str) const;
		void	showChannelsUser(int fd)const;

		int		handleNewUser(struct pollfd pollfd);
		int		selectChannel(uint32_t index, struct pollfd pollfd);


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