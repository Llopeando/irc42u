#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>

#include "defines.hpp"

class Client
{
	public:
		Client(std::string hostname);
		Client(const Client &client);
		~Client();
		
		//void operator=(const Client &client);
		
		Client& operator=(const Client& client);
		std::string	getUserMask()const;
		std::string	getUsername()const;
		std::string getHostname()const;
		std::string	getAwayMsg()const;
		std::string	getNickname()const;
		//bool		getNewClient() const;
		//bool		getInputBlock()const;
		bool		getAwayStatus() const;

		uint8_t		getState() const;
		uint32_t	getChannel()const;
		//uint32_t	getLastMsgIdx()const;
		uint32_t	getRole()const;

		void		setRole(uint8_t role);
		void		setAwayStatus(bool isAway);
		void		setAwayMsg(std::string away_msg);
		void		setUsername(std::string username);
		void		setNickname(std::string nickname);
		void		setPassword(std::string password);
		//void		setNewClient(bool check);
		//void		setLastMsgIdx(uint32_t msgIdx);
//
		//void		addLastMsgIdx(uint32_t num);
		//void		resetLastMsgIdx();
		bool		checkPassword(std::string pass) const;
		
	private:
		std::string nickname;
		std::string username;
		std::string password;
		std::string away_msg;
		std::string hostname;
	//	uint32_t lastMsgIndex; //ultimo mensaje leido
		uint8_t channel; // en todo caso seria un vector porue puede estar en varios
		uint8_t role;
		uint8_t state;
		bool	isAway;
		//bool	inputBlock;
		//bool	newClient;
};

#endif
