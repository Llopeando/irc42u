/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defines.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ullorent <ullorent@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 17:43:44 by ullorent          #+#    #+#             */
/*   Updated: 2023/07/31 13:39:10 by ullorent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINES_H
#define DEFINES_H

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include "Utils.hpp"

#define SERVER_FAILURE -1

#define CL_STATE_OK 0
#define CL_STATE_INPUT 1
#define CL_STATE_LS 2
#define CL_STATE_REQ 3

#define CL_STATE_ACTIVE 1
#define CL_STATE_INACTIVE 0

#define CL_USER 0
#define CL_OPER 2

#define RCVBUFSIZE 1024

#define VERSION "AOIRC Version: 1.0"

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


#define START_CMD_NAMESPACE namespace cmd {
#define END_CMD_NAMESPACE }

#define START_ERROR_NAMESPACE namespace error{
#define END_ERROR_NAMESPACE }

#define START_SERVER_DATA_NAMESPACE namespace sd{
#define END_SERVER_DATA_NAMESPACE }

#define START_ANONYMOUS_NAMESPACE namespace {
#define END_ANONYMOUS_NAMESPACE }


typedef struct activeIndex {
	bool		registered;
	uint32_t	index;
}t_activeIndex;

enum eFlags{
	eSuccess = 0,
	eError = 1,
	eNoSuchFunction = 4,
	eExited = 8,
	eBack = 16,
	eRemoveChannel = 32,
	eRemoveClientChannel = 64,
};

#endif