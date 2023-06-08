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
#include <set>

#include <cerrno>
#include <system_error>

#include "Utils.hpp"
#include "defines.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "color.h"
#include "UsersData.hpp"
#include "ErrorHandler.hpp"
#include "OperBlock.hpp"

class Server;
#define SERVER_NAME "AOIRC Server" ///cambiar??
#define COMMANDS 14
#define CAP_COMMANDS 6

#define MOTD "\n " \
			"         _____                           _______                           _____                            _____                            _____          \n" \
			"         /\\    \\                         /::\\    \\                         /\\    \\                          /\\    \\                          /\\    \\         \n" \
			"        /::\\    \\                       /::::\\    \\                       /::\\    \\                        /::\\    \\                        /::\\    \\        \n" \
			"       /::::\\    \\                     /::::::\\    \\                      \\:::\\    \\                      /::::\\    \\                      /::::\\    \\       \n" \
			"      /::::::\\    \\                   /::::::::\\    \\                      \\:::\\    \\                    /::::::\\    \\                    /::::::\\    \\      \n" \
			"     /:::/\\:::\\    \\                 /:::/~~\\:::\\    \\                      \\:::\\    \\                  /:::/\\:::\\    \\                  /:::/\\:::\\    \\     \n" \
			"    /:::/__\\:::\\    \\               /:::/    \\:::\\    \\                      \\:::\\    \\                /:::/__\\:::\\    \\                /:::/  \\:::\\    \\    \n" \
			"   /::::\\   \\:::\\    \\             /:::/    / \\:::\\    \\                     /::::\\    \\              /::::\\   \\:::\\    \\              /:::/    \\:::\\    \\   \n" \
			"  /::::::\\   \\:::\\    \\           /:::/____/   \\:::\\____\\           ____    /::::::\\    \\            /::::::\\   \\:::\\    \\            /:::/    / \\:::\\    \\  \n" \
			" /:::/\\:::\\   \\:::\\    \\         |:::|    |     |:::|    |         /\\   \\  /:::/\\:::\\    \\          /:::/\\:::\\   \\:::\\____\\          /:::/    /   \\:::\\    \\ \n" \
			"/:::/  \\:::\\   \\:::\\____\\        |:::|____|     |:::|    |        /::\\   \\/:::/  \\:::\\____\\        /:::/  \\:::\\   \\:::|    |        /:::/____/     \\:::\\____\\\n" \
			"\\::/    \\:::\\  /:::/    /         \\:::\\    \\   /:::/    /         \\:::\\  /:::/    \\::/    /        \\::/   |::::\\  /:::|____|        \\:::\\    \\      \\::/    /\n" \
			" \\/____/ \\:::\\/:::/    /           \\:::\\    \\ /:::/    /           \\:::\\/:::/    / \\/____/          \\/____|:::::\\/:::/    /          \\:::\\    \\      \\/____/ \n" \
			"          \\::::::/    /             \\:::\\    /:::/    /             \\::::::/    /                         |:::::::::/    /            \\:::\\    \\             \n" \
			"           \\::::/    /               \\:::\\__/:::/    /               \\::::/____/                          |::|\\::::/    /              \\:::\\    \\            \n" \
			"           /:::/    /                 \\::::::::/    /                 \\:::\\    \\                          |::| \\::/____/                \\:::\\    \\           \n" \
			"          /:::/    /                   \\::::::/    /                   \\:::\\    \\                         |::|  ~|                       \\:::\\    \\          \n" \
			"         /:::/    /                     \\::::/    /                     \\:::\\____\\                        |::|   |                        \\:::\\____\\         \n" \
			"        /:::/    /                       \\::/    /                       \\::/    /                         \\:|   |                         \\::/    /         \n" \
			"        \\::/____/                         \\/____/                         \\/____/                           \\|___|                          \\/____/          \n"



struct Commands{
	std::unordered_map<std::string, void (Server::*)(clientIt index, std::vector<std::string> &arguments)> funcmap;
	std::unordered_map<std::string, void (Server::*)(clientIt index, std::vector<std::string> &arguments)> cap_funcmap;
	//std::string cmd[COMMANDS];
	//void (Server::*func[COMMANDS])(clientIt index, std::vector<std::string> &arguments);
	//std::string cap_cmd[CAP_COMMANDS];
	//void (Server::*cap_func[CAP_COMMANDS])(clientIt index, std::vector<std::string> &arguments);
};



class Server
{
	public:
		typedef std::chrono::system_clock t_chrono;

		Server(t_serverInput *serverInput);
		~Server();

		void	run();
		//void	setSocket(t_serverInput *serverCreateInfo);
		void	printServerStatus() const;
		std::string	getName()const;
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
		//void	sendMsgUser(clientIt it, const std::string &str) const;
		void	setCommands();

		void	deleteChannel(uint32_t channel);



		//COMMAND FUNCTIONS
		void	oper(clientIt index, std::vector<std::string> &arguments);
		void	pass(clientIt index, std::vector<std::string> &arguments);
		void	nick(clientIt index, std::vector<std::string> &arguments);
		void	user(clientIt index, std::vector<std::string> &arguments);
		void	join(clientIt index, std::vector<std::string> &arguments);
		void	privmsg(clientIt index, std::vector<std::string> &arguments);
		
		void	part(clientIt index, std::vector<std::string> &arguments);
		void	topic(clientIt index, std::vector<std::string> &arguments);
		void	list(clientIt index, std::vector<std::string> &arguments);

		
		void	notice(clientIt index, std::vector<std::string> &arguments);
		void	quit(clientIt index, std::vector<std::string> &arguments);
		void	mode(clientIt index, std::vector<std::string> &arguments);
		void	names(clientIt index, std::vector<std::string> &arguments);
		void	whois(clientIt index, std::vector<std::string> &arguments);
		void	who(clientIt index, std::vector<std::string> &arguments);
		void	motd(clientIt index, std::vector<std::string> &arguments);
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
		std::string serverName;
		std::deque<Channel> channels;
		UsersData data;
		Commands commands;
		ErrorHandler errorHandler;

		OperBlock operators;
	
};

#endif