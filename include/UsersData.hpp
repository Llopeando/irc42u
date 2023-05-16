#ifndef USERDATA_HPP
#define USERDATA_HPP

#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <fcntl.h>


#include "Client.hpp"
#include "defines.h"

typedef struct s_serverInput{
	struct sockaddr_in address;
	std::string password;
}t_serverInput;


class UsersData{
	public:
		UsersData();
		UsersData(t_serverInput serverInfo);
		~UsersData();

		pollfd *getPollfdData();
		clientIt findUsername(std::string argument);

		void	addClient(pollfd clientPollfd, Client newClient);

		//override de operadores [] para acceder a polfds y a array de clients
		Client& operator[](clientIt indexAct);
		const Client& operator[](clientIt indexAct) const;
		pollfd &operator[](pollfdIt idx);
		const pollfd &operator[](pollfdIt idx) const;

		uint32_t size()const;
	
		///////OBSOLETE DOWN
	


		//void newTempUser(int fd); 
		//void newRegisteredUser(uint32_t indexAct);
		//
		//uint32_t findUsername(const std::string &username) const;
		//
		////int		getFd(uint32_t indexAct)const;
		//void	popTemp(uint32_t indexAct);
		//
		//void findTmpIndexAndRest(uint32_t indexTemp, uint32_t num);
		
	//	std::vector<struct pollfd>&	Pollfd();
	//	std::vector<t_activeIndex>&	Actives();
	//	std::vector<Client>&	Registered();
	//	std::vector<Client>&	TempClients();
	private:
		void	setSocket(pollfd &server, t_serverInput &serverInfo);
		
		std::vector<struct pollfd> pollfds;
		std::vector<Client> clients;
};

#endif