#ifndef SERVERDATASTRUCTS_H
#define SERVERDATASTRUCTS_H

#include <iostream>

namespace sd{
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
}t_serverInput;
}

#endif