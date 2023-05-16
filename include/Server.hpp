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

#include <cerrno>
#include <system_error>

#include "defines.h"
#include "Channel.hpp"
#include "Client.hpp"
#include "color.h"
#include "UsersData.hpp"


void printIp();
std::string joinStr(std::vector<std::string>& arguments, uint32_t index);
std::vector<std::string> split(const std::string &string, char c);
std::vector<std::string> splitIrcPrameters(const std::string &string, const char c);

class Server;

#define COMMANDS 6
#define CAP_COMMANDS 6


struct Commands{
	std::string cmd[COMMANDS];
	void (Server::*func[COMMANDS])(clientIt index, std::vector<std::string> &arguments);
	std::string cap_cmd[CAP_COMMANDS];
	void (Server::*cap_func[CAP_COMMANDS])(clientIt index, std::vector<std::string> &arguments);
};



class Server
{
	public:
		Server(t_serverInput *serverInput);
		~Server();

		void	run();
		//void	setSocket(t_serverInput *serverCreateInfo);
		void	printServerStatus() const;
	
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
		void	sendMsgUser(clientIt it, const std::string &str) const;
		void	setCommands();


		//COMMAND FUNCTIONS
		void	nick(clientIt index, std::vector<std::string> &arguments);
		void	user(clientIt index, std::vector<std::string> &arguments);
		void	join(clientIt index, std::vector<std::string> &arguments);
		void	part(clientIt index, std::vector<std::string> &arguments);
		void	privmsg(clientIt index, std::vector<std::string> &arguments);
		void	notice(clientIt index, std::vector<std::string> &arguments);
		void	quit(clientIt index, std::vector<std::string> &arguments);
		void	topic(clientIt index, std::vector<std::string> &arguments);
		void	mode(clientIt index, std::vector<std::string> &arguments);
		void	names(clientIt index, std::vector<std::string> &arguments);
		void	whois(clientIt index, std::vector<std::string> &arguments);
		void	kick(clientIt index, std::vector<std::string> &arguments);
		void	away(clientIt index, std::vector<std::string> &arguments);
		void	invite(clientIt index, std::vector<std::string> &arguments);
		void	ping(clientIt index, std::vector<std::string> &arguments);
		void	cap(clientIt index, std::vector<std::string> &arguments);
		uint32_t	findChannel(const std::string &name) const;

		//COMMAND CAP FUNCTIONS
		void	cap_req(clientIt index, std::vector<std::string> &arguments);
		void	cap_ls(clientIt index, std::vector<std::string> &arguments);
		void	cap_end(clientIt index, std::vector<std::string> &arguments);
		void	cap_ack(clientIt index, std::vector<std::string> &arguments);
		void	cap_nak(clientIt index, std::vector<std::string> &arguments);

		int	status;
		t_serverInput serverInfo;
		std::deque<Channel> channels;
		UsersData data;
		Commands commands;
		
		
		
		//////////OBSOLETE DOWN


		
		//
		//bool	checkServerPassword(Client *newClient, uint32_t index, std::string &input);
		//void	loginChoice(Client *newClient, uint32_t index, std::string &input);
		//bool	selectUsername(Client *newClient, uint32_t index, std::string &input);
		//bool	checkPassword(Client *newClient, uint32_t index, std::string &input);
		//void	selectNickname(Client *newClient, uint32_t index, std::string &input);
		//void	tempToRegistered(uint32_t indexAct);
//
//
		//void	command(uint32_t indexAct, std::string &input);
		//void	microshell(uint32_t index, std::string &input);
		//void	nickname_edit_m(uint32_t indexAct, std::string &argument);
		//void	password_edit_m(uint32_t indexAct, std::string &argument);
		//void	role_edit_m(uint32_t indexAct, std::string &argument);
		//void	join_channel_m(uint32_t indexAct, std::string &input);
		//void	leave_channel_m(uint32_t indexAct, std::string &input);
		//void	leave_server_m(uint32_t indexAct, std::string &argument);
		//void	susurro_m(uint32_t indexAct, std::string &argument);
		//void	createChannel_m(uint32_t indexAct, std::string &argument);
		//void	deleteChannel_m(uint32_t indexAct, std::string &argument);
		//void	myInfo_m(uint32_t indexAct, std::string &argument);
//
		//uint32_t	findUsername(const std::string &username)const;
//
		//bool checkAdmin(Client *client);
//
		////bool	writeInChannel(std::string input);
		//void	newClient();
		//void	cleanUp();
	
		//void	showChannelsUser(int fd)const;

		//int		handleNewUser(struct pollfd pollfd);
		//int		selectChannel(uint32_t index, struct pollfd pollfd);
//
		//bool	assertClientPassword(uint32_t indexActive, const std::string &password) const;
	
		//int	accptConnection;
	
			//struct sockaddr_in address;
			//std::string password;
			
	/*	std::deque<struct pollfd> pollfds;
		std::deque<t_activeIndex> actives;

		std::deque<Client> tempClients;
		std::deque<Client> registered;
	*/
};



#endif