#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "defines.h"


#include <iostream>
//#include <cstring>
//#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
//#include <cstdio>
#include <poll.h>
#include <deque>
#include "Client.hpp"
#include "UsersData.hpp"

class Channel
{
	public:
		Channel(std::string name, std::string username, UsersData *data);
		~Channel();
		
		std::string			getName()const;
		void 				addClient(uint32_t indexAct);
		void				removeClient(uint32_t indexAct);
		void 				broadcast(uint32_t indexAct, std::string const &msg);
		void				refresh(uint32_t indexAct);
		void				flushLog(Client &user, uint32_t user_pos);
		
	private:
		uint32_t numOfUsers;
		UsersData *data;
		std::string name;
		std::deque<uint32_t> users;
		//std::deque<uint32_t>msgIndexUsr; //ultimo mensaje leido
		std::string creator;
		std::deque<std::string> msg_log;
		
		//Punteros a arrays de server	
		uint32_t	findUser(uint32_t indexAct)const;
		void		sendMsgUser(uint32_t user_pos, std::string const &str) const;
		void		sendMsgChannel(uint32_t user_pos, std::string const &str, bool val);
		void		sendInfoChannel(uint32_t user_pos, std::string const &str);
		
};

#endif