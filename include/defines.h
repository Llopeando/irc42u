#ifndef DEFINES_H
#define DEFINES_H

#include <iostream>

#define SERVER_FAILURE -1

#define CL_STATE_SERVER_PASSWORD 0
#define CL_STATE_LOG_IN 1
#define CL_STATE_SELECT_USERNAME 2
#define CL_STATE_INTRODUCE_PERSONAL_PASSWORD 3
#define CL_STATE_SELECT_NICKNAME 4
#define CL_STATE_LOBBY 5
#define CL_STATE_IN_CHANNEL 6

#define CL_STATE_ACTIVE 1
#define CL_STATE_INACTIVE 0

#define CL_USER 0
#define CL_ROOT 1

#define RCVBUFSIZE 1024

namespace color {
	const std::string reset		= "\033[0m";
	const std::string black		= "\033[30m";
	const std::string red		= "\033[31m";
	const std::string green		= "\033[32m";
	const std::string yellow	= "\033[33m";
	const std::string blue		= "\033[34m";
	const std::string magenta	= "\033[35m";
	const std::string cyan		= "\033[36m";
	const std::string white		= "\033[37m";

	const std::string boldblack		=  "\033[1m\033[30m";
	const std::string boldred		=  "\033[1m\033[31m";
	const std::string boldgreen		=  "\033[1m\033[32m";
	const std::string boldyellow	=  "\033[1m\033[33m";
	const std::string boldblue		=  "\033[1m\033[34m";
	const std::string boldmagenta	=  "\033[1m\033[35m";
	const std::string boldcyan		=  "\033[1m\033[36m";
	const std::string boldwhite		=  "\033[1m\033[37m";
	
	const std::string boldgreenback =  "\033[1m\033[42m";
}

#include <sys/socket.h>
#include <netinet/in.h>

typedef struct activeIndex
{
	bool		registered;
	uint32_t	index;
}t_activeIndex;

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

#endif
