#ifndef SERVERDATA_HPP
#define SERVERDATA_HPP

#include <iostream>
#include <vector>

#include "Config.h"
#include "ServerDataStructs.h"

#include "Client.hpp"
#include "OperBlock.hpp"
#include "Channel.hpp"

START_SERVER_DATA_NAMESPACE

class ServerData{
	public:
		ServerData(t_serverInput &serverInfo);
		~ServerData();

		pollfd		*getPollfdData();

		uint32_t	pollfdSize() const {return pollfds.size();}

		clientIt	findUsername(const std::string& argument) const;
		clientIt	findNickname(const std::string& argument) const;
		clientIt	findNicknameBack(const std::string& argument) const;

		void	addClient(pollfd clientPollfd, Client newClient);
		void	transferIndex(clientIt index, const std::string& nickname);
		void	backClient(clientIt index); // fom client to back
		void	forwardClient(const std::string& nickname); //from back to client
		void	removeClient(clientIt index);

		//override de operadores [] para acceder a polfds y a array de clients
		//clients
		Client& operator[](clientIt idx){return clients[idx];}
		const Client& operator[](clientIt idx)const {return clients[idx];}
		pollfd &operator[](pollfdIt idx){return pollfds[idx];}
		const pollfd &operator[](pollfdIt idx) const{return pollfds[idx];}
		Channel &operator[](channIt idx){return channels[idx];}
		const Channel &operator[](channIt idx) const{return channels[idx];}

	private:
		std::vector<struct pollfd> pollfds;
		std::vector<Client> clients;
		std::vector<Client> back;//no se puede acceder con operators
		std::deque<Channel> channels;
		OperBlock operators;
		std::string serverName;


		void	setSocket(pollfd &server, t_serverInput &serverInfo);
};

END_SERVER_DATA_NAMESPACE


#endif