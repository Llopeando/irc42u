#ifndef USERDATA_HPP
#define USERDATA_HPP

#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <fcntl.h>


#include "Client.hpp"
#include "defines.hpp"

typedef struct s_serverInput{
	struct sockaddr_in address;
	std::string password;
	std::string IP;
}t_serverInput;


class UsersData{
	public:
		UsersData();
		UsersData(t_serverInput serverInfo);
		~UsersData();

		pollfd *getPollfdData();
		clientIt findUsername(std::string argument) const;
		clientIt findNickname(std::string argument) const;
		clientIt findNicknameBack(const std::string argument) const;
		void	addClient(pollfd clientPollfd, Client newClient);

		void	transferIndex(clientIt index, const std::string& nickname);
		void	backClient(clientIt index); // fom client to back
		void	forwardClient(const std::string& nickname); //from back to client
		void	removeClient(clientIt index);
		//override de operadores [] para acceder a polfds y a array de clients
		Client& operator[](clientIt indexAct);
		const Client& operator[](clientIt indexAct) const;
		pollfd &operator[](pollfdIt idx);
		const pollfd &operator[](pollfdIt idx) const;

		uint32_t size()const;
	private:
		void	setSocket(pollfd &server, t_serverInput &serverInfo);
		
		std::vector<struct pollfd> pollfds;
		std::vector<Client> clients;
		std::vector<Client> back;//no se puede acceder con operators
};

#endif