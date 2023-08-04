#ifndef SERVERDATASTRUCTS_H
#define SERVERDATASTRUCTS_H

#include "defines.h"

#include <iostream>
#include <netinet/in.h>
#include "defines.h"

START_SERVER_DATA_NAMESPACE

enum Authentified{
	eNone = 0,
	ePass = 1,
	eNick = 2,
	eUser = 4,
	eAuthentified = ePass | eNick | eUser,
};

struct clientIt{
	uint32_t index;
	clientIt(int value = 0):index(value){};
	operator uint32_t() const {
		return index;
	}
	clientIt& operator=(uint32_t value){
		index = value;
		return *this;
	}
	clientIt& operator++() {
		++index;
		return *this;
	}
	clientIt operator++(int) {
		clientIt tmp(*this);
		++index;
		return tmp;
	}
};

struct backIt{
	uint32_t index;
	backIt(int value = 0):index(value){};
	operator uint32_t() const {
		return index;
	}
	backIt& operator=(uint32_t value){
		index = value;
		return *this;
	}
	backIt& operator++() {
		++index;
		return *this;
	}
	backIt operator++(int) {
		backIt tmp(*this);
		++index;
		return tmp;
	}
};

struct pollfdIt{
	uint32_t index;
	pollfdIt(int value = 0):index(value){};
	operator uint32_t() const {
		return index;
	}
	pollfdIt& operator=(uint32_t value){
		index = value;
		return *this;
	}
	pollfdIt& operator++() {
		++index;
		return *this;
	}
	pollfdIt operator++(int) {
		pollfdIt tmp(*this);
		++index;
		return tmp;
	}
};

struct channIt{
	uint32_t index;
	channIt(int value = 0):index(value){};
	operator uint32_t() const {
		return index;
	}
	channIt& operator=(uint32_t value){
		index = value;
		return *this;
	}
	channIt& operator++() {
		++index;
		return *this;
	}
	channIt operator++(int) {
		channIt tmp(*this);
		++index;
		return tmp;
	}
};

typedef struct s_serverInput{
	struct sockaddr_in address;
	std::string password;
	std::string IP;
	std::string serverName;
	std::string version;
	std::string versionComments;
	std::string chantypes;
	std::string prefix;
	uint32_t modes;
	uint32_t chanlimit;
	uint32_t nicklen;
	uint32_t userlen;
	uint32_t hostlen;
	uint32_t topiclen;
	uint32_t kicklen;
	uint32_t channellen;
	uint32_t maxusers;
	uint32_t maxuserschan;
}t_serverInput;

END_SERVER_DATA_NAMESPACE

#endif