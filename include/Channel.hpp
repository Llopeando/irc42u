#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "defines.hpp"
#include "Utils.hpp"

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
		typedef std::chrono::system_clock t_chrono;

		Channel(std::string name, std::string username, UsersData *data);
		~Channel();
		
		std::string			getCreator() const;
		std::time_t			getCreationDate()const;
		void				setCreationDate(std::time_t);
		std::string			getName()const;
		std::string			getUserList()const;
		void 				addClient(clientIt index);
		void				removeClient(clientIt indexAct);
		void 				broadcast(clientIt sender, std::string const &msg);
		//void				refresh(uint32_t indexAct);
		//void				flushLog(Client &user, uint32_t user_pos);
		void				setTopic(std::string topic);
		std::string			getTopic( void)const;
		uint32_t			getNumUser( void)const;
		
	private:
		uint32_t	findUser(clientIt indexAct)const;
		void		sendInfoChannel(uint32_t user_pos, std::string const &str);


		uint32_t numOfUsers;
		UsersData *data;
		std::string name;
		std::string topic;
		std::deque<uint32_t> users;
		//std::deque<uint32_t>msgIndexUsr; //ultimo mensaje leido
		std::string creator;
		std::deque<std::string> msg_log;
		std::time_t creationDate;
};

#endif