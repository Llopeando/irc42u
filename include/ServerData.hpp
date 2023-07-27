/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerData.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ullorent <ullorent@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 17:43:32 by ullorent          #+#    #+#             */
/*   Updated: 2023/07/27 17:43:33 by ullorent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERDATA_HPP
#define SERVERDATA_HPP

#include <iostream>
#include <vector>
#include <unordered_map>

#include "Config.h"
#include "ServerDataStructs.h"

#include "Client.hpp"
#include "Channel.hpp"


/*
*	Public:
*		class ServerData
*/


START_SERVER_DATA_NAMESPACE

class ServerData{
	public:
		ServerData(t_serverInput &serverInfo);
		~ServerData();

		pollfd		*getPollfdData();
		std::string getName() const;
		std::string getPassword() const {return password;}
		const t_serverInput& getConfig() const{return config;}

 		uint32_t	pollfdSize() const {return pollfds.size();}


		char		*getCreationDate()const;

		clientIt	findUsername(const std::string& argument) const;
		uint32_t	findUsernameBack(const std::string& argument) const;
		clientIt	findNickname(const std::string& argument) const;
		clientIt	findNicknameBack(const std::string& argument) const;

		void	addClient(pollfd clientPollfd, Client newClient);
		void	transferIndex(clientIt index, const std::string& nickname);
		void	backClient(clientIt index); 
		void	forwardClient(const std::string& nickname); 
		void	removeClient(clientIt index);

		//OPERATOR BLOCK

		std::string getOperList();
	//	void addOper(std::sting user, std::string pass);
		bool findOper(const std::string &user)const;
		bool checkOperPass(const std::string &user, const std::string& pass) const;

		//CHANNEL OPERATIONS 

		void		addChannel(std::string name, std::string username, ServerData &serverData);
		uint32_t	getNumOfChannels() const;
		uint32_t	getNumOfClients() const;
		void 		removeClientChannels(sd::clientIt index);
		void		deleteChannel(uint32_t channel);
		uint32_t	findChannel(const std::string &name) const;

		// OPERATOR OVERRIDES

		Client& operator[](clientIt idx){return clients[idx];}
		const Client& operator[](clientIt idx)const {return clients[idx];}
		pollfd &operator[](pollfdIt idx){return pollfds[idx];}
		const pollfd &operator[](pollfdIt idx) const{return pollfds[idx];}
		Channel &operator[](channIt idx){return channels[idx];}
		const Channel &operator[](channIt idx) const{return channels[idx];}

	private:
		std::vector<struct pollfd> pollfds;
		std::vector<Client> clients;
		std::vector<Client> back; //only operators
		std::deque<Channel> channels;
		std::unordered_map<std::string, std::string> operblock;
		std::string serverName;
		std::string password;
		std::time_t creationDate;

		t_serverInput &config;

		void	setSocket(pollfd &server, t_serverInput &serverInfo);
};

END_SERVER_DATA_NAMESPACE


#endif