/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ullorent <ullorent@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 16:14:43 by ecamara           #+#    #+#             */
/*   Updated: 2023/04/04 19:37:29 by ullorent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <sys/socket.h>
#include <netinet/in.h>

#define SERVER_FAILURE -1
#define CL_STATE_SERVER_PASSWORD 0
#define CL_STATE_SELECT_USERNAME 1
#define CL_STATE_INTRODUCE_PERSONAL_PASSWORD 2
#define CL_STATE_SELECT_NICKNAME 3
#define CL_STATE_LOBBY 4
#define CL_STATE_IN_CHANNEL 5

#define CL_STATE_ACTIVE 1
#define CL_STATE_INACTIVE 0

typedef struct s_serverInput{
	struct sockaddr_in address;
	std::string password;
}t_serverInput;


typedef struct tempClient
{
	std::string username;
	std::string nickname;
	std::string password;
}tempClient;

typedef struct activeIndex
{
	bool		registered;
	uint32_t	index;
}activeIndex;

typedef struct s_commands
{
	std::string cmd[8];
	void (Server::*func[8])(uint32_t index);
}t_commands;
