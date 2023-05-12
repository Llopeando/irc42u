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

#include "defines.h"
#include "Channel.hpp"
#include "Client.hpp"
#include "color.h"
#include "UsersData.hpp"

std::vector<std::string> split(const std::string &string, char c);
std::vector<std::string> splitIrcPrameters(const std::string &string, const char c);

class Server;

#define COMMANDS 9

typedef struct s_commands
{
	std::string	cmd[COMMANDS];
	void (Server::*func[COMMANDS])(uint32_t index, std::string &argument);
}t_commands;



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

		void	saveNick(std::vector<std::string> &arguments, Client &newUser);
		void	saveUser(std::vector<std::string> &arguments, Client &newUser);

		void	checkFds(int events);
		void	handleEvents(pollfdIt index);

		std::string	readTCPInput(int client_fd);
		void	sendMsgUser(int fd, const std::string &str) const;

		int	status;
		t_serverInput serverInfo;
		std::deque<Channel> channels;
		UsersData data;
		t_commands commands;
		
		
		
		//////////OBSOLETE DOWN


		
		//void	setCommands();
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
		//uint32_t	findChannel(const std::string &name) const;
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