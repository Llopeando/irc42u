#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>

#include "defines.hpp"

class Client
{
	public:
		Client();
		Client(std::string hostname);
		Client(const Client &client);
		~Client();
		
		
		Client& operator=(const Client& client);
		std::string	getUserMask()const;
		std::string	getUsername()const;
		std::string getHostname()const;
		std::string	getAwayMsg()const;
		std::string	getNickname()const;
		bool		getAwayStatus() const;
		bool		getAuthentificied()const;
		uint8_t		getState() const;
		uint32_t	getRole()const;

		void		setAuthentificied(bool status);
		void		setRole(uint8_t role);
		void		setAwayStatus(bool isAway);
		void		setAwayMsg(std::string away_msg);
		void		setUsername(std::string username);
		void		setNickname(std::string nickname);
		void		setPassword(std::string password);
		
		
		//void		setLastMsgIdx(uint32_t msgIdx);
		//uint32_t	getLastMsgIdx()const;
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
	// en todo caso seria un vector porue puede estar en varios
		uint8_t role;
		uint8_t state;
		bool	isAway;
		bool	authentificied;
		//bool	inputBlock;
		//bool	newClient;
};

#endif
