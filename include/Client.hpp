#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>

#include "defines.h"

class Client
{
	public:
		Client();
		Client(const Client &client);
		~Client();
		
		//void operator=(const Client &client);
		
		Client& operator=(const Client& client);

		uint8_t getState()const;
		void	setState(uint8_t state);

		uint32_t	getRole()const;
		void		setRole(uint8_t role);
	
		void		setUsername(std::string username);
		std::string	getUsername()const;
		void		setNickname(std::string nickname);
		std::string	getNickname()const;
		void		setPassword(std::string password);
		bool		getNewClient() const;
		void		setNewClient(bool check);
		uint32_t	getChannel()const;
		uint32_t	getLastMsgIdx()const;
		bool		getInputBlock()const;

		
		void		addLastMsgIdx(uint32_t num);
		void		setLastMsgIdx(uint32_t msgIdx);
		void		resetLastMsgIdx();
		bool		checkPassword(std::string pass) const;
		
	private:
		std::string nickname;
		std::string username;
		std::string password;
		uint32_t lastMsgIndex; //ultimo mensaje leido
		uint8_t channel;
		uint8_t role;
		uint8_t state;
		bool	active;
		bool	inputBlock;
		bool	newClient;
		
};

#endif
