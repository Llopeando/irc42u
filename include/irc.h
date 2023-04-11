/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecamara <ecamara@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 16:14:43 by ecamara           #+#    #+#             */
/*   Updated: 2023/04/11 19:01:32 by ecamara          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include "Client.hpp"


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

typedef struct s_serverInput{
	struct sockaddr_in address;
	std::string password;
}t_serverInput;


typedef struct s_tempClient
{
	Client		tempClient;
	bool		newClient;
}t_tempClient;

typedef struct activeIndex
{
	bool		registered;
	uint32_t	index;
}t_activeIndex;

class Server;

typedef struct s_commands
{
	std::string cmd[8];
	void (Server::*func[8])(uint32_t index, std::string argument);
}t_commands;

#include "Server.hpp"